<h1 align="center">
	📖 cub3D
</h1>

```
This project is inspired by the world-famous Wolfenstein 3D game, which
was the first FPS ever. It will enable you to explore ray-casting. Your goal will be to
make a dynamic view inside a maze, in which you’ll have to find your way.
```

## Description
Cub3D is all about bringing that classic raycasting game feeling to life. It’s a fun way to dive into the world of graphic design while figuring out how math fits into computer graphics. At least we did a lot of research beforehand, which helped us later on to make a 2D space feel like a 3D world more easily. That being said I'd argue cub3D isn’t just another project; it’s a creative playground where coding and math team up to create some awesome interactive experiences!

## Compile and Run

There are two versions of the program, the mandatory part and the bonus part. Bonus part adds a minimap, cats, start and end screens, as well as the ability to rotate the view by moving the mouse.

`❗note` : we cannot gurantee that it will work on windows devices, it should run on mac and linux just fine.

To compile the mandatory part, `cd` into the cloned directory and:

```shell
make
```

To compile the bonus part, `cd` into the cloned directory and:

```shell
make bonus
```

To run the program:

```shell
./cub3D <path/to/map.cub>
```

The program takes a map file as an argument. Maps are available in the `maps` directory. There are good maps which the program should run smoothly with, and bad maps which the program should reject.
For example:

- `./cub3D maps/good/map_1.cub` should run.

- `./cub3D maps/bad/empty.cub` should print an error and abort.

## Controls

Controls for movement and rotation are:

- `W`: move forward
- `S`: move backward
- `A`: move left
- `D`: move right
- `left arrow`: rotate left
- `right arrow`: rotate right
- `mouse`: rotate by moving the mouse (bonus only)
