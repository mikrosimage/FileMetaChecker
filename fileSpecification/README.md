How to write a specification file ?
===================================

Specification dictionary
------------------------

### Element keys

* __id__     _(compulsory)_
* __label__  _(compulsory)_
* type
* size
* values
* range
* map
* group
* groupSize
* repetition
* requirement
* endian
* optional
* ordered
* display
* file

### Data types

* unknown _(default)_
* ascii
* hexa
* raw
* int8
* uint8
* int16
* uint16
* int32
* uint32
* int64
* uint64
* float
* double
* ieeeExtended


Write a specification file
--------------------------
FileMetaChecker works as a comparator between a file and a reference, which is generally a format specification. In order to access this reference, a specification in a JSON file must be provided to FileMetaChecker. This specification file must follow some writing rules, as follow.

### Element description
Most of the time, it is suitable to describe a file format as a row of basic elements, which represent different data types. These elements can have parent-child relationships, can be compulsory, optional, repeated, and so on. FileMetaChecker use this kind of basic element trees to describe file formats, and define Element objects to represent those basic elements. At the end of the comparison process, each Element includes a status :

* Not checked
* Valid
* Invalid
* Invalid but optional
* Invalid for unordered
* Invalid but skip
* Invalid for iteration
* Invalid group for iteration
* Pass over
* Skip


#### Compulsory fields : ID and label
In the format description context, a basic Element is a JSON Object, and MUST be at least composed with an ID and a label. The ID must be unique, while the label is a free description of the Element's meaning (human readable as far as possible).

	{
		"id": "thisIsABasicElement",
		"label": "This is a basic element",
	}
> Minimum Element definition:


#### Data types
Elements should have a type to describe the data it represents. FileMetaChecker handles the following types:

* ascii : ASCII character string
* hexa : Hexadecimal value (string)
* int8 : signed 8-bit integer
* uint8 : unsigned 8-bit integer
* int16 : signed 16-bit integer
* uint16 : unsigned 16-bit integer
* int32 : signed 32-bit integer
* uint32 : unsigned 32-bit integer
* int64 : signed 64-bit integer
* uint64 : unsigned 64-bit integer
* float : 32-bit floating-point
* double : 64-bit floating-point
* ieeeExtended : 80-bit floating-point
* raw : raw data

An Element may have no specified type, but it will not represent any data.

	{
		"id": "thisIsABasicElement",
		"label": "This is a basic element",
		"type": "uint32"
	}


#### Element value
An Element may have specified values. If its type is "ascii" or "hexa", the Element must have one or several possible values. If the value of the data extract from the file is not one of these specified values, the Element is not valid.

Single value:

	{
		"id": "thisIsABasicElement",
		"label": "This is a basic element",
		"type": "ascii",
		"values": "abcdefgh"
	}

Several values:

	{
		"id": "thisIsAnotherBasicElement",
		"label": "This is another basic element",
		"type": "hexa",
		"values": [
			"00B3",
			"FFD8"
		]
	}

In the multi-values case, every values must have the same length.

If the type is a number (integer or floating-point), a value range may be defined, and can be composed with several ranges. Each range must contain a "min" value, a "max" value or both. If the Element's value is in the range, it becomes Valid, Invalid otherwise. If no range is defined, the Element value is only extracted from the file and its status becomes "Pass Over".
	
	{
		"id": "thisIsABasicElement",
		"label": "This is a basic element",
		"type": "int8",
		"range": [
			{ max": -10 },
			{ "min": -2, "max": 4 },
			{ "min": 6 },
		]
	}

> Here, the Element'value must be less than -10, between -2 and 4, or more than 6 to be Valid.


If the type is "raw", no value can be specified, data are extracted directly from file, and the status becomes "Pass Over".

#### Element's size
The size of each Element must be specified to deduce the size of data to be extracted from the file. However, the size specification depends on the type of the Element:
* For number elements (integer and floating-point), the data size of the Element is directly deduce from the type, so it must not be specified.
* For "ascii" and "hexa" elements, the data size is deduce from the specified value(s).
* For "raw" element, the size MUST be specified with the "size" field. This field handles 'Python' expressions, and can refer to another Element's value.

	{
		"id": "element1",
		"label": "Element 1",
		"type": "uint8"
	},
	{
		"id": "rawData",
		"label": "Raw data",
		"type": "raw",
		"size": "element1 + 2"
	}


#### Groups
Elements may contain other elements. That is what define the "group" fields, which allow to describe arrays of elements :

	{
		"id": "root",
		"label": "Root element",
		"type": "ascii",
		"values": "ROOT",
		"group": [
			{
				"id": "child1",
				"label": "First child"
			}, 
			{
				"id": "child2",
				"label": "Second child"
			}
		]
	}

The group size may be specified, to ensure its value, with the "groupSize" key. If the group size real value is more than the specified value, the root Element receive a WARNING message, if it is less than the specified value (some bytes are missing), the root Element becomes Invalid and receive a ERROR message.

	{
		"id": "root",
		"label": "Root element",
		"groupSize": "6",
		"group": [
			{
				"id": "child1",
				"label": "First child",
				"type": "uint16"
			}, 
			{
				"id": "child2",
				"label": "Second child",
				"type": "uint16",
				"values": "STOP"
			}
		]
	}

Groups may also be unordered, meaning that the order of the children may be different than specified.

	{
		"id": "root",
		"label": "Root element",
		"ordered": false,
		"group": [
			{
				"id": "child1",
				"label": "I'll be the third child",
				"type": "ascii",
				"values": "3"
			}, 
			{
				"id": "child2",
				"label": "I'll be the first child",
				"type": "ascii",
				"values": "1"
			}, 
			{
				"id": "child2",
				"label": "I'll be the second child",
				"type": "ascii",
				"values": "2"
			}
		]
	}

> NB: the unordered children must be "ascii" or "hexa" Elements.


#### Optional Elements
An Element may be optional : if its status is not Valid, it is skipped.

	{
		"id": "optionalElement",
		"label": "This is an optional element",
		"type": "ascii",
		"values": "value",
		"optional": true
	}

> NB: this optionality is not supported for numbers and raw Elements without specified range (because of the "Pass Over" status).


#### Repetitions
An Element can be repeated several times in a file, so, in order to simplified its writing, the number of repetition may be defined, as a single value or a range. The repeated Element is Valid if it repetition number is equal to the specified value (eq. include in the specified range). The "repetition" field handles 'Python' expressions and element references.

Single repetition value:

	{
		"id": "repeatedElement",
		"label": "This is a repeated element",
		"type": "ascii",
		"repetition": "3"
	}

Repetition range:

	{
		"id": "repeatedElement",
		"label": "This is a repeated element",
		"type": "ascii",
		"repetition": [
			{ "min": 3, "max": 6 }
		]
	}

> NB: repetitions are not avaible for number and raw Elements.

> NB: if the Element is optional and repeated, it loses it optionality if it appears one (or more) time in the file.


#### Endianness
Endianness may be specified with the "endian" key, and the "big" and "little" values. The default value is "big".


#### Requirement
It is possible to specify conditions for the presence of an element. If the condition is not true, the Element is skipped.

	{
		"id": "element1",
		"label": "Element 1",
		"type": "uint16"
	},
	{
		"id": "element2",
		"label": "Element 2",
		"type": "ascii",
		"values": "Hello!"
		"requirement": "element1 is 3"
	}

#### Maps
Elements values may be combined to labels, to make their meaning clearer.

	{
		"id": "element",
		"label": "Element",
		"type": "uint32",
		"map": [
			{   "1": "this means 1" },
			{   "2": "this means 2" },
			{  "23": "this means 23" },
			{ "136": "this means 136" }
		]
	}

> NB: left values must be in quotes, whatever the Element's type.
