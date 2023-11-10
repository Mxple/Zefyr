## Syntax 
`()` Parenthesis:
 Parenthesis can be used in 3 ways:
1. Grouping or precedence override
2. Turning a pure function call into a lambda
3. Turning operators like `+` and `%` into lambdas

`|` Pipe:
Pipes have 3 uses:
1. `||` represents OR
2. `|` represents a pattern-matching guard
```
def func a1 a2 as
  | a1 > a2 do...
  | a1 < a2 do...
  | do ...
```
1.  `[|]` pipes inside brackets or curly braces represent set builder notation's "such as"
	- `[i in 0.. | i % 2 == 0]` infinite list of even numbers


## Data Types 
There are two types of data types: built-in and prelude. Built-in types come with the language itself. Prelude types are defined in the prelude.

#### Built-in Types
Zefyr includes 4 built-in types:
1. `int32`
2. `double`
3. `bool`
4. `ptr`
All types are 64-bits and represented using NaN-boxing. There is additionally one special type: `array`. Arrays can live on the stack and are defined as follows:
`...|arr_start size| | | | | | | | |size end| | | | | |...`
Each `| |` represents 8 bytes on the stack. The array start only holds information about the array: how long it is and that the following x chunks of memory belong to the array. It can be initialized as follows
```
>>> arr1 = [30]   // 30 is the size of the array
[nil, nil, nil, ..., nil]
>>> arr2 = [1, 2, 3, 4, 42]
[1, 2, 3, 4, 42]
```
The array type can be thought of as basically declaring `size` number of  variables, each with a name corresponding to an array index. 

## Features 
#### UFCS, Piping, and Function Composition
Universal function call syntax allows for all functions which take arguments to take in their first argument in the form of `arg1.func`. In other words, method-call syntax without objects or methods, just plain old functions. In the 1st pass of the parser, `arg1` is evaluated to an expression and `func` is evaluated to a function call. If `arg1` is a data type which has `func` as a method, the VTable lookup evaluates the method instead as it has higher priority. In fact, methods implicitly have `self` as their first argument.

UFCS does have a problem of precedence though. Since `.` has the highest precedence, some expressions become ambiguous. We can solve this with parenthesis, *or* we can use the `|>` symbol which has the lowest precedence .
```
>>> join [1,2,3] [2,3,4] .map (^2)
[1, 2, 3, 4, 9, 16]
>>> join [1,2,3] [2,3,4] |> map (^2)
[1, 4, 9, 4, 9, 16]
```

This allows for the familiarity of OOP-style languages with method chaining syntax, but also a bit of functional style piping or currying, albeit, it is a bit of a workaround compared to ML languages.

#### Guards
Pattern matching guards allow for cleaner if else chains that match on a scope's local variables. Guards support fall through and read like `switch` statements.
```
// guards used inside a function
def isPrime n as
  | n < 2 
  | n % 2 == 0 do // statement 1 falls thru to here
    return false
  | do            // empty conditional evals to true
    for i in 3..n.sqrt.toInt do
	  if n % i == 0 return false
  true
```
Guards can be represented as if-else trees. The above example in pseudo code:
```
if (n < 2) {

}
if (n < 2 || n % 2 == 0) {
  return false;
}
else {
...
}
```
Notice the fall through. We can break or return out of a `do` block to simulate `else if`

#### Iterators and Ranges
We can use `x..y` where `x` and `y` are integers to generate a range. There's also some special ranges: `..` is `INT_MIN..INT_MAX`, `x..` is `x..INT_MAX` and reverse for `..y`. These are all shorthand for `range x y 1` or `range x y -1`. 

An iterable is any data type that implements the `Iterable` trait:
```
trait Iterable as
  iterDoneFlag = false  // this flag is set when the iteration is exhausted
  currentIter = nil     // pointer to current element
  def iter() as         // iteration starts here. iter is called once and
                        // should return the first object
  def next() as         // returns reference to next element

data Iterator as
  ptr = nil
  def Iterator as
    
```

```
for i in range 1 10 1 do
  // something
```
is equivalent to 
```
iterable = range 1 10 1
i = iterable.iter
while (iterable.iterDoneFlag == false) do
  // something
  i = iterable.next

// drop iterable and i from the scope
```
Range implementation may look like:
```
@impl Iterable
data Range as
  start = end = step = nil    // assignment is right associative
  iterDoneFlag = false
  currentIter = nil
  
  def Range start end step as
    start = start
    end   = end
    step  = step
	
  def iter as
	currentIter = start      // will return the value of start
	
  def next as
    currentIter += step
    if step > 0 && currentIter >= end || step < 0 && currentIter <= end do 
      iterDoneFlag = true
      return nil
	currentIter 
```

#### List Comprehension 
In a similar vein, we have list comprehension. The pattern is `iterable | condition`. This generates another iterator, which can be infinite.
```
// all primes (up to INT_MAX)
primes = List [2]
primes.extend (i in range 3 INT_MAX 2 | i.isPrime)
```
There's a little bit of nuance with lazy evaluation. Does list comprehension evaluate to a loop or to an iterable???

## Example Programs
Simple syntax demo
```
// lambdas must be pure functions
def map arr lambda as
  newArr = [arr.size]
  for i, item in arr.enumerate do  // unpacking values
	newArr[i] = item.lambda        // UFCS
  newArr

def filter arr lambda as
  newArr = [arr.size]
  for i, item in arr.enumerate do
    if item.lambda do
      newArr[i] = item.copy
  newArr

def isPrime n as
  if n <= 1 do return false
  for i in 2..n.sqrt .toInt do
    if n % i == 0 do return false
  true

>>> myArr = [0, 1, 5, 9, 13, 19, 20, 42, 50]
[0, 1, 5, 9, 13, 19, 20, 42, 50]
>>> myArr.map (_+2)    // underscore is the empty operand/argument
[2, 3, 7, 11, 15, 21, 22, 44, 52]
>>> myArr.filter (isPrime)
[5, 13]
>>> myArr.filter (isPrime) |> map (^2)
[25, 169]
```

