This is a fork of the [scrot](http://en.wikipedia.org/wiki/Scrot) utilitiy.

Most of the code has been rewritten and restructured for the sake of clarity.

Two changes have been made:

- `-w` - The `-w` flag has been added. When scrot is started with this flag, it
  waits for the user to click on a window and will then take a screenshot of
  the window.
- `-s` - The `-s` flag has been modified. Clicking on a window will no longer
  take a screenshot of the window. Instead, when you press and release the left
  mouse button and then move the mouse, a rectangle will appear. Clicking and
  releasing again will take a screenshot of the marked rectangle. You can
  achieve the same by pressing the mouse button, moving the mouse, and releasing
  the mouse button. This change has been made so that the `-s` flag is easier to
  use with touchpads.
