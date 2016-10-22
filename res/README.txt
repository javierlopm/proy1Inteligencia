# Proyecto 1 - Inteligencia artificial I

##Integrantes:

11-10396 Nabil  Márquez

11-10683 Isaac  González

11-10552 Javier López

----

# Resultados

En esta entrega se encuentran los directorios ejercicio3 y ejercicio4 y ejercicio5, los cuales contienen los resultados de las corridas de las corridas de este proyecto en formato csv.

Lo siguientes son los contenidos de los directorios:

* ejercicio3: incluye las corridas de un algorítmo de búsqueda ciega (dfs acotado e iterativo sin heurística) para los siguientes problemas:
    - Pancake con 16 panquecas
    - 11-Puzzle
    - TopSpin 16-4
    - Tower of Hanoi 14-4

* ejercicio4: incluye las corridas de A* y IDA* en formato csv con la heurística Manhattan y la abstracción PDB 5+5+5 para 15-puzzle, y heurística GAP para Pancake
    - 15Puzzle
    - Pancake 28
* ejercicio5: Análogo al ejercicio 4, mismás heurísticas y problemas pero utilizando el algoritmo WIDA* con peso 1.5 y 2.

Adicionalmente, para el ejercicio 1 y 2 se realizó un dfs acotado con y sin recortes de ramas sobre los problemas Pancake 24 y 28, 11-puzzle, TopSpin 22-4 y 22-6 y Torre de Hanoi 14-4 y 16-4, en donde se contó el número de nodos visitados y, posteriormente el factor de ramificación obtenido. Las respuestas de estos problemas se encuentran en tablas1.md y tablas1.pdf.

# Implementación

Para la resolución de estos problemas, creamos los siguientes archivos, los cuales son cada uno de los algoritmos utilizados y deben estar ubicados en la carpeta global del directorio psvn:
    - dfs_count.cpp: dfs acotado, realiza conteo de nodos
    - id_dfs.cpp: búsqueda en dfs acotada, de forma iterativa se aumenta la profundidad buscada hasta encontrar el objetivo.
    - best_first_search.cpp: algoritmo necesario para realizar búsqueda A*.
    - a_star_pdb  : el cual implementa a* con 3 abstración para 15 puzzle
        + 15puzzle_abs0.txt
        + 15puzzle_abs1.txt
        + 15puzzle_abs2.txt
    - ida_man.cpp : IDA* para la resolución de 15 puzzle
    - ida_gap.cpp : IDA* para la resolución de pancakes
    - wida_man.cpp: 15 puzzle con WIDA*
    - wida_gap.cpp: pacakes con WIDA*
    - wida_pdb.cpp: wida con pdb

También tuvimos que crear los archivos psvn para 11puzzle y 15puzzle, el resto de los problemas fueron creados mediante sus respectivos programas generadores.

Por último, se crearon 3 abstracciones para el problema de 15 puzzle, en el cual se realizaron proyecciones de variables hasta obtener tres subconjuntos disjuntos del problema inicial.

Todos los archivos de implementación mencionados anteriormente se encuentran en el directorio src.


# Problemas encontrados

Durante la ejecución de best_first_search encontramos problemas en la resolución de 15 puzzle con la heurística manhattan, ya que el state_map que almacena las distancias de cada nodo y su color se llenaba antes de poder alcanzar la solución. Para intentar resolver este problema se creó el archivo a_star_manhattan, el cual intenta atacar este problema, manejando arreglos de mapas de estados, en vez de un mapa único, pero no se logro resolver un mayor número de instancias que en la versión anterior, este archivo se encuentra incluido en el directorio src.
