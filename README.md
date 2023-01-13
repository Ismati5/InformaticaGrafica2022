# InformaticaGrafica2022

Ismael Tienda Hernando  796919

Adrián Yago Hernández   796441


---

#### Ejemplos de imágenes

![concurso_796441_796919](https://user-images.githubusercontent.com/79214939/212430982-9a69cda7-2384-415a-945d-3d4d1259e075.png)
Final work, rendered in 1920x1080 - Path Tracing with shadows - 64 rpp
<br/><br/>
![bear](https://user-images.githubusercontent.com/79214939/209251178-e99bdc11-4f6a-4684-bde1-aa2081c05530.png)
Bear rendered in 1920x1080 - Path Tracing with shadows - 312 rpp

<br/><br/>
---
<br/><br/>

#### Compilación

Para la compilación se recomienda utilizar ejecutar el fichero "makefile.bat" si se está ejecutando el proyecto en windows. 

Si se quiere compilar manualmente se puede utilizar el siguiente comando:

```cpp
g++ point.cpp direction.cpp render.cpp matrix4.cpp vect4.cpp vect3.cpp object.cpp sphere.cpp station.cpp primitive.cpp light.cpp plane.cpp camera.cpp triangle.cpp square.cpp -o render -std=c++17 -O1
```

---


#### Ejecución path tracer

Para la ejecución se utiliza el siguiente comando:

```cpp
render -r <filename> <rays per pixel>
```

Los parámetros son los siguientes:

- "filename": Hace referencia al fichero de extensión .dat en el que se encuentra la escena. El fichero debe de pasarse al comando sin la extensión del archivo

- "rays per pixel": Son los rayos por pixeles con los que se renderizará la escena.

Un ejemplo de uso sería el siguiente

```cpp
./render -r cornell_1 32
```

---

#### Ejecución photon mapping

Para la ejecución se utiliza el siguiente comando:

```cpp
render -p <filename> <rays per pixel> <max photons>
```

Los parámetros son los siguiente:

- "filename": Hace referencia al fichero de extensión .dat en el que se encuentra la escena. El fichero debe de pasarse al comando sin la extensión del archivo

- "rays per pixel": Son los rayos por pixeles con los que se renderizará la escena.

- "max photons": Es el número de fotones que se guardarán en el mapa de fotones.

Un ejemplo de uso sería el siguiente

```cpp
./render -p cornell_pm 32 10000
```

En la carpeta "scenes" se encuentran varios ficheros .dat con varias escenas con las que hacer pruebas.
