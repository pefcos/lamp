# lamp and lampi
A boolean (lamp) based esoteric programming language, and its interpreter.

***

## Lamp documentation

### Values in lamp



### Variable types
Since it is a boolean based language, all values in lamp are either on (true) or off (false). With that in mind, there are no types such as integers or strings, instead, the lamp language has two types: lamps and switches. A lamp is a variable that is either on or off (just like a real life lamp). An example of lamp declaration would be: `lamp example on`. This creates a lamp called example with the value on. 

A switch is analogous to a real life switch, that has an on and off position. Each position in a switch can hold a value, but all switches have only two positions, no more, no less. A basic example of a switch declaration would be: `switch example (on off)`. This creates a switch that holds the value on in the first position (called the off position) and the value off in the second position (called the on position). To access the first value we can type `example.off` while to access the second value we can type `example.on`. Simple, right? Now for the most interesting part. Switches can hold another switch instead of an on/off value, making nested switches not only viable, but essential. For example, we could declare a switch within a switch like this: `switch example (on (off on))`. Now, when accessing the off position, the value would be not on or off, but the `(on off)` switch. To access an element within the inner switch (off element for the example), we can type the following: `example.off.off`.

### Displaying values
