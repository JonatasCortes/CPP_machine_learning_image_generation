# CPP_machine_learning_image_generation
The objective of this project is to replicate the algorithm implemented in  https://github.com/SebastianCharmot/Genetic-Algorithm-Image-Recreation with c++, with the most basic image processing library, and maybe solve the asymptote issue the original creators of the algorithm encountered.

## How to run

The image processing libraries used in this project are [stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h) and [stb_image_write.h](https://github.com/nothings/stb/blob/master/stb_image_write.h), both header-only.

> [!INFO]
> Due to the large number of code present in the header files, I chose not to include them in the online repository, but in the links above you can have easy access to them.

Notice that in the directory `include/` there is a `.gitkeep` file. It is there just to preserve de directories structure and can be removed once you have alocated the header files `stb_image.h` and `stb_image_write.h` there.

At the end of this process you should have a similar directories structure:
```
CPP_machine_learning_image_generation/
├── include/
│   ├── stb_image_write.h
│   └── stb_image.h
├── src/
│   ├── ...
│   └── ...
├── .gitignore
├── main.cpp
└── README.md
```
