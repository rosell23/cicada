<div align="left">
<img src="logo.jpg" alt="Cicada logo" width="100" height="100">
</div>

<div align="right">
<p>Translations :<a href="./README.md">English</a></p>
</div>

<div align="center">
<h1><i>The Cicada OS</i></h1>
<kbd><a href="#acerca-de">Acerca de</a></kbd>
<kbd><a href="#instalacion">Instalación</a></kbd>
</div>

## <span style="font-variant:small-caps;">Acerca de</span> ####################

_Cicada_ es una implementación de [L4 ✦](https://os.inf.tu-dresden.de/L4/)
Es un _microkernel_ de _segunda generación_ y, por lo tanto, tiene
**_IPC_ muy rápido** como otros microkernels de la familia.

Está escrito en _[Forth ✦](https://forth-standard.org/)_,
y soporta xx86/64, IA-32 (también llamado _i386_, aún que
no es factual, nadie usa una Intel 80386 en 2025...), y
[RISC-V ✦](https:/riscv.org/).

Este repositorio contiene el, microkernel en sí (Cicada<sup>L4</sup), un
dialecto de _Forth_ llamado _MForth_ (micro-Forth or motherfuc**** Forth,
depende de como the sientas), y ⅈℂ𝕚𝕔𝕒𝕕𝕒 (leé el siguiente capitulo).

<!-- TODO: traducir más -->

## ⅈℂ𝕚𝕔𝕒𝕕𝕒 ####################################################################

iCicada (estilizado en blackboard-bold: ⅈℂ𝕚𝕔𝕒𝕕𝕒) es la implementación de referencia
de cualquier espacio de usuario de Cicada.   Es un espacio de usuario
completo y usable que aprovecha todas las funciones que Cicada<sup>L4</sup> ofrece.
A menudo se usa como referencia para sistemas más grandes, más que como un sistema
independiente. Es un sistema tipo UNIX.

## <span style="font-variant:small-caps;">Installation</span> #################

Primero que nada, no ejecutarás esto en hardware real. El soporte nativo de
Cicada es inexistente, a menos que uses ~hardware muy específico~.

Ahora, compila el proyecto. Debes tener instaladas algunas dependencias:

+ [Cros-compilador de GCC para tu arcitecture ✦](https://wiki.osdev.org/GCC_Cross-Compiler)
+ [GForth](https://gforth.org/), para inicializár MForth
+ [`make`](https://gnu.org/software/make/).
+ Tú aceptación con el [EULA](#EULA).
+ Una cópia del código fuente con [git](https://git-scm.org/).

Ejecuta:

```sh file=term
% make -Cicicada		          # Generar archivos ICicada
% make userfs			          # Crear sistema de archivos de usuario
% tar -cvjf cpfs.tar.bz2 gen/user/ gen/fs # Crear sistema de archivos para copiar
% make disk ARCHIVE=cpfs.tar.bz2          # Crear el disco
```

> [!NOTE]
> `%` es un indicador de prompt. A menudo es `$` u otro símbolo.

## EULA #######################################################################

Lee la [licencia/EULA aquí](./COPYING) (Inglés).

Es básicamente la EUPL pero con copyleft leve, y mas _intenacional_.

