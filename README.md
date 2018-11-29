# chip8
Chip 8 interpreter, written in C and SDL. 

Requires SDL2. If you don't have libsdl copied to C:Windows\System, then you'll need to include libsdl.dll in the project directory.

Simply compile with make, and then run "emu <filename.ch8>".

NOTE: This is my first emulator, and also my first project using SDL, so the code probably isn't quite as good as it could be. That said, this is a work in progress, and I still have a number of things that i want to do.
        
TODO: 

      Implement correct timing (emulation cycle at ~540hz, but sound/delay supposed to be ~60hz?!?!?

      Update to use super-chip 48 instructions 
      
      Make my own beep noise instead of printing \a.
            
      Sprites are supposed to "wrap" around when they go out of bounds of the screen, need to work this out.
      
FIXED: 

       Score now updating correctly (it wasn't working becuase i accidentally left out a break statement! How embarrassing...)
       
       Added option to turn off beeping noise (it gets annoying listening to it over and over again when testing!)
       
       Implemented opcode Fx0A (stop execution until keypress)
      
      
      
