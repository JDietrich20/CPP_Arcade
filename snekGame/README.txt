Simple Snake Game implemented with C++

ncurses.h > conio.h due to portability issues and better terminal handling.

Further implementations:
- refine the bounds check (bottoma and top?)
- Move tail after moving the head (or carefully attach first segment after movement).
- Initialize new tail segment when growing.
- Initialize tail array at startup to prevent garbage collisions.

