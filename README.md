# TP2

hola :) 

es mejor vector o puedo usar multimap
com carajo se ordenan los ataques y eso
puedo hacer pokedex static con los archivos (funciona igual pero queda mas fachero)

"Estos pokemones deben ser reconocidos por el Pokedex simplemente utilizando
una instancia de la clase Pokemon. La idea es reconocer su tipo y mostrar por
pantalla su información." a que se refiere con esto

es normal que al deserializar se de vuelta el orden

tengo que agregar que chequee que la pokedex tenga cosas cuando le cargo pokemones del .bin
tengo que checkear los nombres de los archivos y eso para evitar repticiones etc


### Consideracion a preguntar del 3

Yo hago que el sleep lo haga fuera del lock del recurso compartido, asi no hago que todo el resto de actores tengan que esperar a que termine el sleep para poder hacer su task.
Nose si esto esta bien o no, va a las chapas asi esta buenisimo

---

Link que capaz pongo despues en el informe para que mariano no me venga a decir que lo hice con gpt:

https://www.geeksforgeeks.org/multithreading-in-cpp/
https://youtu.be/hXKtYRleQd8?si=sePO8DyLzCXCmVuZ
https://www.tek-tips.com/threads/mutex-arrays.1431058/ (wtf puedo tener hasta una matriz de mutexes)