# lamp and lampi
A boolean based esoteric programming language, and its interpreter.

You can download the latest lampi release in the [releases](https://github.com/pefcos/lamp/releases) section.

## Lamp documentation

### Syntax
Lamp's syntax is much simples than other programming languages, as lamp has no indicators of code blocks such as "{}" or even ":". Lamp also is indentation independent, which means that lamp code has only one requirement after all: the keywords need to be spaced and sequential.

### Variable types
Since it is a boolean based language, all values in lamp are either on (true) or off (false). With that in mind, there are no types such as integers or strings, instead, the lamp language has two types: lamps and switches. A lamp is a variable that is either on or off (just like a real life lamp). An example of lamp declaration would be: `lamp example on`. This creates a lamp called example with the value on. 

A switch is analogous to a real life switch, that has an on and off position. Each position in a switch can hold a value, but all switches have only two positions, no more, no less. A basic example of a switch declaration would be: `switch example (on off)`. This creates a switch that holds the value on in the first position (called the off position) and the value off in the second position (called the on position). To access the first value we can type `example.off` while to access the second value we can type `example.on`. Simple, right? Now for the most interesting part. Switches can hold another switch instead of an on/off value, making nested switches not only viable, but essential in lamp programming. For example, we could declare a switch within a switch like this: `switch example (on (off on))`. Now, when accessing the on position, the value would be not on or off, but the `(off on)` switch. To access an element within the inner switch (off element for the example), we can type the following: `example.off.off`. A switch element that is not another switch is called a switch lamp, as it also stores on and off values.

It is possible to build switches in a variety of ways. We could build a switch from an element of a bigger switch by typing the following code:
```
switch example_a (on (off on))
switch example_b example_a.on
```
This code creates a switch called example_b with the value (off on), based on the element in the on position of the switch example_a. We can also reference individual lamps, switch lamps and even switches inside a switch declaration, for example:
```
switch example_a (on (off on))
lamp example_b on
switch example_c ((example_b example_a.on.off) example_a)
```
This code generates a switch called example_c with the value ((on off) (on (off on))), based on the references to the lamp example_b, the switch lamp example_a.on.off, and the switch example_a.

Lastly, it is also possible to use a reduced notation to declare switches, this notation consists of typing the '.' and 'o' characters to represent off and on values respectively, between a pair of perentheses. For example, the switch (on (off (on on))) could be declared as `switch example (o.oo)`. No spaces are allowed inside the reduced notation. 

### Displaying values
There are two types of value displays, a block display and a word display. Word displays are inferred while block displays have to be explicited. For example, consider the following code:
```
lamp example on
display example
```
This code creates a lamp called example with the value on, and prints the word "on" on the screen. But displaying information to the user becomes boring if you can only say "on" and "off". This is why the block display exists! An example of a block display would be:
```
lamp example on
display block example
```
This would print a "█" on the screen. Even if word displays are inferred, they can also be explicited, an example of this would be `display word example`.

Now you know about block displays, but they are kind of useless for single lamps. Let's talk about displaying switches! They are displayed the same way as lamps and can also be displayed in blocks and words. Pairing block displays with switches gives you a creative way to draw on the display (see examples/print_lamp.lamp for an example on block display drawing).

### Inverse values
In lamp, a variable can have its value inverted using the "-" symbol before its name, without spaces in between. For example, we may want to display the inverse of a lamp's content. To display the inverse of the content of the lamp "example", we can write `display -example`. If example is "on", it will display "off", and if it is "off", lampi will display "on".

### Comments
Comments are needed so that the programmer can better understand the code written, and every major programming language provides a way to write them. In lamp, to comment a section, you can type "##" to open and close a comment section. An example would be `## this is a lamp comment ##`.

### Deleting variables
Variables in lamp can also be deleted by the programmer by typing the word "delete", followed by the variable type and the variable name. An example of deleting a lamp variable is `delete lamp example`. This can be used to increase the programmer's control over memory usage, deleting lamps and switches that will not be used anymore.

### Namespaces
The lamp programming language supports namespaces. Namespaces are added to variables by inserting the namespace of the variable and its name after a ":". An example would be `lamp ex:example on`, where "ex" is the namespace and "example" is the name of the lamp. Namespacing your variables allows you to have variables with the same name in multiple places on your code without getting lost. For example, the code below:
```
lamp a:example on
lamp b:example off
display a:example
```
will display "on".

A thing to note is that all variables have namespaces, even if implied. The default namespace for lamps is "lamp", while the default namespace for switch is "switch". The line of lamp code `lamp example on` will actually create a lamp example inside the "lamp" namespace, therefore it is the same as writing `lamp lamp:example on`. Namespacing may seem unnecessary given what you know about lamp so far, but it will come in handy in the next section.

### Circuits
Now for the hard part! You know how in other programming languages we have functions or methods to divide code? In lamp, we call those circuits, and understanding how they work is paramount for you to be a good lamp programmer. Circuits are declared with the "circuit" keyword before the circuit name and end with the `ground` keyword. The code `circuit example` will define a circuit starting in the next statement, ending only when the `ground` of the circuit is found. A basic example of circuit definition would be:
```
circuit example_circ
  lamp example (on off)
  display example
ground
```
Ok, so you can write a circuit, great, now you need to learn how to call them! To execute code inside a circuit you have to type the word "power" followed by the circuit name and the "on" word, or a reference to a lamp with value on. If you type `power example_circ on`, you can start the code inside the example circuit we made before. If you typed `power example_circ off`, the circuit example_circ would not be called.

Where can I call a circuit? Anywhere, including inside itself. This gives versatility to the syntax, as circuit definitions between lines of code will be ignored by lampi until it is called (and will be ignored forever if there is no "power" statement).

Why do I have to put "on" in every circuit powering? Because, as you may have noticed, lamp language has no "if" statement, like most other languages do. The way to write an if statement in lamp is a clever trick involving circuits. The code:
```
power example_circ example_lamp
circuit example_circ
  lamp new on
  display new
ground
```
will only execute the circuit "example_circ" if the lamp "example_lamp" is on. Otherwise, it will not. This is the same as writing something like `if(example_lamp)` in other languages.

Now about namespaces and circuits. There is no variable scope in lamp, so it means that the code:
```
circuit example_circ
  lamp example on
  display example
ground

lamp example off
example_circ
display example
```
will display "off" two times, as both lamp assignment statements change the lamp "lamp:example". Using namespaces we can change the code to:
```
circuit example_circ
  lamp example_circ:example on
  display example_circ:example
ground

lamp example off
example_circ
display example
```
and now the displaying will show a "on" and a "off", since they both modify a lamp called example, but in different namespaces.

Consgratulations! Now you too can start coding in lamp.

## Lampi

Lampi is the lamp interpreter for linux operating systems, which can be invoked running "lampi" with the path to the .lamp file with your lamp code. An usage example would be `lampi example.lamp`, to interpret the "example.lamp" file.

Lampi binaries are provided in this repository in the [releases](https://github.com/pefcos/lamp/releases) section, but they may not work in every computer, so you can build your own lampi by cloning this repository and executing `make lampi` on the repository's root directory.

### Usage options

A debugging option is avaliable in lampi, you can access it by typing the "-d"/"--debug" argument after the file name. This will print messages on every lamp command execution and make debugging easier.

There is also a "-h"/"--help" option, to invoke it type `lampi -h`. It will display help about the usage of lampi.

You can also invoke lampi with the "-v"/"--version" option, like `lampi -v`. This will display the version of lampi you are currently using. If you need a different version of lampi than the one you are currently using, you can access the [releases](https://github.com/pefcos/lamp/releases) section and download the lampi version you need.
