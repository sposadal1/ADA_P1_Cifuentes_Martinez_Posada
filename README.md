# Practica 1: Fuerza Bruta y Backtracking 

Este repositorio contiene la solucion tecnica, experimentos e informe final correspondiente a la Practica 1 del curso Analisis y Diseno de Algoritmos (ADA).

---

## Integrantes del Equipo

* Alejandro Cifuentes Arroyave
* Juan Daniel Martinez Fernandez
* Samuel Posada Londono

Semilla del equipo: 2472
(Calculada mediante la suma del codigo ASCII de los apellidos concatenados `cifuentesmartinezposada` modulo 100000).

---

## Organizacion del Proyecto

El proyecto esta estructurado modularmente en C++17 sin bibliotecas externas complejas. El modulo FB (Fuerza Bruta) implementa la enumeracion exhaustiva de cadenas sobre alfabetos A1 y A2 y la comparacion contra ataques por diccionario sintetico. El modulo BT (Backtracking) evalua la construccion incremental de contrasenas con poda por factibilidad (espacio insuficiente y repeticion consecutiva), complementado por un modulo de Programacion Dinamica para la validacion teorica exacta en espacios combinatorios masivos (n >= 8).

---

## Requisitos de Compilacion

* Lenguaje: C++17
* Compilador: g++ 9.0 o superior (compatible con Windows MinGW / WSL2 / Linux)
* Flags de compilacion obligatorias: -std=c++17 -O2

### Comando de Compilacion Unificado

Ejecute el siguiente comando desde la raiz del repositorio:

```bash
g++ -std=c++17 -O2 -o ada_p1 src/*.cpp
```

En Windows:

```bash
g++ -std=c++17 -O2 -o ada_p1.exe src/*.cpp
```

---

## Reproduccion de Experimentos

Para reproducir los resultados reportados en las Tablas 1 y 2 del informe, utilice los siguientes comandos unificados:

### 1. Ejecutar Modulo Fuerza Bruta (Modulo FB)

Ejecuta las 5 instancias asignadas por la semilla 2472 (n=4, 5, 6), realiza el ataque por diccionario y exporta las metricas en results/fb_resultados.csv:

```bash
./ada_p1 --modo fb
```

### 2. Ejecutar Modulo Backtracking (Modulo BT)

Ejecuta la calibracion en n=6 (Con Poda vs. Sin Poda) y realiza la verificacion de soluciones exactas con Programacion Dinamica:

```bash
./ada_p1 --modo bt
```

En Windows, reemplace `./ada_p1` por `.\ada_p1.exe` en los comandos anteriores.
