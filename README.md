# luxatron

Luxatron (LXT) is a small and robust games engine designed for use on the mobile platform. It has been developed by a veteran game engine programmer and is designed to be low level, composable and un-intrusive.

See LICENSE.TXT for more information.

Currently supported features:
- iPhone/iPod platform - all code runs on ARM and Intel platforms.
- OpenGL ES 1.5 rendering support.
- Collada model conversion to native LXTM format.
- Rendering hierarchical models.
- Rendering bitmap fonts.
- Sound support.
- Built in CPU profiling and memory leak checking.
- Maths library
- Binary serialization library (through Archive & Buffer class)
- Unit tests that run on ARM, Intel and PPC platforms.

Upcoming features for first release:
- Batched mesh rendering (to minimize state change costs).
- More optimal vertex formats.
- ARM VFP math acceleration.
- 2D GUI rendering.
- Particle rendering.

On the code
===========

Almost all classes are unit tested, and unit tests also check for memory leaks. The only known leaks occur in native (objective-c) interface for file handling.

Throughout the Maths library a four component vector (Vec4) is the only vector type permitted to perform maths operations, it is left to the programmer to manage the point or direction forms (w=1, w=0) of the Vec4.

The Vec3 type is provided for storage purposes only, 

Questions?
==========

Contact: justin@monkeystylegames.com

