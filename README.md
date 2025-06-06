# TP2

# Para corre el ej 1:
Pararse en la carpeta build: 

    cd ej1/build

y construir el CMake:

    cmake ..

Una vez creado el CMake, compilar con make y correr con bin/main

# IMPORTANTE:
- Correr con la terminal en pantalla completa y lo mas despejada posible (se recomienda ocultar la barra de los costados)
- En caso de que los pokemones se vean cortados o mal, revisar pokedex.cpp y modificar siguiendo las instrucciones indicadas el valor de MAX_TERM_WIDTH
- Hay cargadas 2 pokedexes que se pueden utilizar para una mejor experiencia:
    - All Unlocked: como su nombre indica, estan todos los pokemones cargados (desbloqueados)
    - tests: estan cargados los primeros 51 pokemones y algunos mas. Se recomienda usar esta para poder probar una pokedex dividida en paginas pudiendo llegar al final de la misma y agregar pokemones
    - Crear una nueva: ademas, esta la opcion de crear una nueva pokedex sin pokemones para probar todo de 0

La parte de consigna esta hecha totalmente por nosotros, sin embargo para poder hacer las imagenes de los pokemones en la terminal se uso una libreria externa "stb_image.h" del siguiente repositorio:

https://github.com/nothings/stb

### Consideracion a preguntar del 3

Yo hago que el sleep lo haga fuera del lock del recurso compartido, asi no hago que todo el resto de actores tengan que esperar a que termine el sleep para poder hacer su task.
Nose si esto esta bien o no, va a las chapas asi esta buenisimo

---

Link que capaz pongo despues en el informe para que mariano no me venga a decir que lo hice con gpt:

https://www.geeksforgeeks.org/multithreading-in-cpp/
https://youtu.be/hXKtYRleQd8?si=sePO8DyLzCXCmVuZ
https://www.tek-tips.com/threads/mutex-arrays.1431058/ (wtf puedo tener hasta una matriz de mutexes)