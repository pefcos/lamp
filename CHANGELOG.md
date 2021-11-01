# Changelog
All changes to Lamp will be documented in this file. To run programs in a specific lamp version, you need the corresponding lampi. All of the lampi versions can be found in the "bin" directory.

To better understand version numbers, this is how Lamp treats them: 
- Major releases will modify the first number in the version numbers (1.0, 2.0, 3.0, etc). These releases may break lamp programs that run well in the previous version and will usually contain a lot of new things that change lamp programming significantly.
- Minor releases will modify the second number in the version numbers (1.0, 1.1, 1.2, etc). These releases add new things to the language and are mostly backwards compatible, which means that a program that ran well in the previous version is likely to run just as well on the new version, and may be incremented to use the new features. 
- Subminor releases will modify the third number in the version numbers (1.0.1, 1.0.2, 1.0.3, etc). These will never break a program that was written according to specification of the previous version and will rarely add something significant. They are mainly intended for small fixes and correction of previous version oversights.

## [1.0]
- Added lamp/switch types.
- Added variable deleting.
- Added displays and block displays.
- Added circuits.
- Added inversions.
- Added comments.
- Added namespaces.

## [1.0.1]
- Added support for tab indentation.
- Unknown/unexpected words now will interrupt program execution.
