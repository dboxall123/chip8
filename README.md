# chip8
Chip 8 interpreter, written in C and SDL. 

Requires SDL2. If you don't have libsdl copied to C:Windows\System, then you'll need to include libsdl.dll in the project directory.

Simply compile with make, and then run "emu <filename.ch8>".

NOTE: This is my first emulator, and also my first project using SDL, so the code probably isn't quite as good as it could be. That said, this is a work in progress, and I still have a number of things that i want to do.

ISSUES: Score in games like pong not updating (read somewhere this is due to incorrect BCD implementation, I'm looking into this).
        When running at 60 fps (which is the speed at which it's supposed to run) it's really slow. This doesn't make much sense!
        
TODO: Solve above issues.
      Update to use super-chip 48 instructions 
      Make my own beep noise instead of printing \a.
      Implement "wait_for_input()" function for opcode 0xFx0A.
      
      
      
