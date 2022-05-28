# Seam-Carving
C implementation of an image processing technique that removes "low energy" pixels while still preserving important features. This project makes use of dynamic programming in order to find the vertical path that results in the lowest energy pixels being removed. 

An example of the seam carving algorithm in action using the C code in this repository:

![Alt Text](https://media.giphy.com/media/5QjK3u1iwzekPRxWtM/giphy.gif)

## The Algorithm

Seam carving makes use of dynamic programming by first assigning an energy value ot each pixel. The energy value is a function of how much a pixel's RGB values differs from its neighbours with a higher energy representing a greater energy. This array of energy values is then proceesed and the path from top to bottom encompassing the least energy is selected as the seam to carve.
