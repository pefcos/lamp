# lamp and lampi
A boolean (lamp) based esoteric programming language, and its interpreter.

## Lamp documentation

### Variable types
Since it is a boolean based language, all values in lamp are either on (true) or off (false). With that in mind, there are no types such as integers or strings, instead, the lamp language has two types: lamps and switches. A lamp is a variable that is either on or off (just like a real life lamp). An example of lamp declaration would be: `lamp example on`. This creates a lamp called example with the value on. 

A switch is analogous to a real life switch, that has an on and off position. Each position in a switch can hold a value, but all switches have only two positions, no more, no less. A basic example of a switch declaration would be: `switch example (on off)`. This creates a switch that holds the value on in the first position (called the off position) and the value off in the second position (called the on position). To access the first value we can type `example.off` while to access the second value we can type `example.on`. Simple, right? Now for the most interesting part. Switches can hold another switch instead of an on/off value, making nested switches not only viable, but essential. For example, we could declare a switch within a switch like this: `switch example (on (off on))`. Now, when accessing the off position, the value would be not on or off, but the `(on off)` switch. To access an element within the inner switch (off element for the example), we can type the following: `example.off.off`.

### Displaying values
There are two types of displays, a block display and a word display. Word displays are inferred while block displays have to be explicited. For example, consider the following code:
```
lamp example on
display example
```
This code creates a lamp called example with the value on, and prints the word "on" on the screen. But interaction with the user becomes boring if you can only say "on" and "off". This is why the block display exists! An example of a block display would be:
```
lamp example on
display block example
```
This would print a "█" on the screen.

Now you know about block displays, but they are kind of useless for lamps. Let's talk about displaying switches! They are displayed the same way as lamps and can also be displayed in blocks and words. Pairing block displays with switches gives you a creative way to draw on the display.

### Namespaces
The lamp programming language supports namespaces. Namespaces are added to variables by inserting the namespace of the variable and its name after a ":". An example would be `lamp ex:example on`, where "ex" is the namespace and "example" is the name of the lamp. Namespacing your variables allows you to have variables with the same name in multiple places on your code without getting lost. For example, the code below:
```
lamp a:example on
lamp b:example off
display a:example
```
will display "on".

A thing to note is that all variables have namespaces, even if implied. The default namespace for lamps is "lamp", while the default namespace for switch is "switch". The line of lamp code `lamp example on` will actually create a lamp example inside the "lamp" namespace, therefore it is the same as writing `lamp lamp:example on`. 
