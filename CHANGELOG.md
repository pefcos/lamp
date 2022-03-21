# Changelog
All changes to Lamp will be documented in this file. To run programs in a specific lamp version, you need the corresponding lampi. All of the lampi versions can be found in the "bin" directory in case newer releases break existing programs.

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
- Comments no longer require spaces after/before the "##"

## [1.0.2]
- Stricter error messages.
- Lampi now keeps track of the line being interpreted, generating clearer error messages.

## [1.1]
- Added support for mixed switch assignment (where switches can be switch elements as well).
- Added reduced switch notation.
- Added switch flexible spacing in writing ('(' and ')' no longer need to be attached to words).
- Added "lamp?" and "switch?" words for handling type checks.

## [1.1.1]
- Added the "namespace" keyword to change lamp and switch namespaces onward.
