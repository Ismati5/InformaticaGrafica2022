# Informatica gráfica

Ismael Tienda Hernando    796919

Adrián Yago Hernández        796441

---

#### Compilación

Para la compilación se recomienda utilizar ejecutar el fichero "makefile.bat" si se está ejecutando el proyecto en windows. 

Si se quiere compilar manualmente se puede utilizar el siguiente comando:

```cpp
g++ point.cpp direction.cpp render.cpp matrix4.cpp vect4.cpp vect3.cpp object.cpp sphere.cpp station.cpp primitive.cpp light.cpp plane.cpp camera.cpp triangle.cpp square.cpp -o render -std=c++17 -O1
```

El programa ha sido desarrollado y probado en windows por lo que no se asegura el buen funcionamiento en otros sistemas.

#### Ejecución photon mapping

Para la ejecución se utiliza el siguiente comando:

```cpp
render -p <filename> <rays per pixel> <max photons>
```

Los parametros son los siguiente:

- "filename": Hace referencia al fichero de extensión .dat en el que se encuentra la escena. El fichero debe de pasarse al comando sin la extensión del archivo

- "rays per pixel": Son los rayos por pixeles con los que se renderizará la escena.

- "max photons": Es el número de fotones que se guardarán en el mapa de fotones.

Un ejemplo de uso sería el siguiente

```cpp
./render -p cornell_pm 32 10000
```

En la carpeta "scenes" se encuentran varios ficheros .dat con varias escenas con las que hacer pruebas.
