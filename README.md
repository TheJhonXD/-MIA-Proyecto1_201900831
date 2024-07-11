# Proyecto de Manejo e Implementación de Archivos

Este proyecto implementa una aplicación de consola para la administración de archivos y particiones utilizando comandos específicos. A continuación, se describen los comandos disponibles.

## Comandos

### MKDISK

Crea un archivo binario que simula un disco duro.

**Sintaxis**:

```bash
mkdisk >size=<tamaño> >path=<ruta> [>fit=<ajuste>] [>unit=<unidad>]
```

**Parámetros**:

- `>size`: (Obligatorio) Tamaño del disco a crear. Debe ser un número positivo mayor a cero.
- `>path`: (Obligatorio) Ruta donde se creará el archivo que representará el disco duro. Se crearán las carpetas necesarias si no existen.
- `>fit`: (Opcional) Ajuste que utilizará el disco para crear las particiones. Valores posibles: `BF` (Best Fit), `FF` (First Fit), `WF` (Worst Fit). Por defecto, `FF`.
- `>unit`: (Opcional) Unidades para el tamaño del disco. Valores posibles: `K` (Kilobytes), `M` (Megabytes). Por defecto, `M`.

**Ejemplo**:

```bash
# Crea un disco de 100 Megabytes en la ruta especificada utilizando el ajuste Best Fit
mkdisk >size=100 >path=/home/user/discos/Disco1.dsk >fit=BF >unit=M
```

### FDISK

Administra las particiones en el archivo que representa el disco duro.

**Sintaxis**:

```bash
fdisk >size=<tamaño> >path=<ruta> >name=<nombre> [>unit=<unidad>] [>type=<tipo>] [>fit=<ajuste>]
```

**Parámetros**:

- `size`: (Obligatorio al crear) Tamaño de la partición a crear. Debe ser un número positivo mayor a cero.
- `path`: (Obligatorio) Ruta del disco en el que se creará la partición. El archivo debe existir.
- `name`: (Obligatorio) Nombre de la partición. No debe repetirse dentro de las particiones del mismo disco.
- `unit`: (Opcional) Unidades para el tamaño de la partición. Valores posibles: `B` (bytes), `K` (Kilobytes), `M` (Megabytes). Por defecto, `K`.
- `type`: (Opcional) Tipo de partición. Valores posibles: `P` (Primaria), `E` (Extendida), `L` (Lógica). Por defecto, `P`.
- `fit`: (Opcional) Ajuste para la partición. Valores posibles: `BF` (Best Fit), `FF` (First Fit), `WF` (Worst Fit). Por defecto, `FF`.

**Ejemplo**:

```bash
fdisk >size=300 >path=/home/user/discos/Disco1.dsk >name=Particion1 >type=P >fit=WF >unit=K
```

### MKFILE

Crea un archivo en el sistema de archivos simulado.

**Sintaxis**:

```bash
mkfile >path=<ruta> [>size=<tamaño>] [>cont=<contenido>] [>r]
```

**Parámetros**:

- `path`: (Obligatorio) Ruta del archivo a crear. Si incluye espacios en blanco, debe encerrarse entre comillas.
- `size`: (Opcional) Tamaño del archivo en bytes. El contenido será números del 0 al 9 repetidos hasta alcanzar el tamaño especificado.
- `cont`: (Opcional) Ruta de un archivo cuyo contenido se copiará en el nuevo archivo.
- `r`: (Opcional) Crea las carpetas padres especificadas en la ruta si no existen. No recibe ningún valor adicional.

**Ejemplo**:

```bash
mkfile >path="/home/user/docs/archivo.txt" >size=100 >r
```

### MKDIR

Crea una carpeta en el sistema de archivos simulado.

**Sintaxis**:

```bash
mkdir >path=<ruta> [>r]
```

**Parámetros**:

- `path`: (Obligatorio) Ruta de la carpeta a crear. Si incluye espacios en blanco, debe encerrarse entre comillas.
- `r`: (Opcional) Crea las carpetas padres especificadas en la ruta si no existen. No recibe ningún valor adicional.

**Ejemplo**:

```bash
mkdir >path="/home/user/docs/nueva_carpeta" >r
```

### CAT

Muestra el contenido de un archivo.

**Sintaxis**:

```bash
cat >fileN=<archivo1> [>fileN=<archivo2>] ...
```

**Parámetros**:

- `fileN`: (Obligatorio) Lista de archivos cuyos contenidos se mostrarán.

**Ejemplo**:

```bash
cat >fileN="/home/user/docs/archivo.txt"
```

### CHMOD

Cambia los permisos de un archivo o carpeta.

**Sintaxis**:

```bash
chmod >path=<ruta> >ugo=<permisos> [>r]
```

**Parámetros**:

- `path`: (Obligatorio) Ruta del archivo o carpeta cuyos permisos se cambiarán.
- `ugo`: (Obligatorio) Permisos que se asignarán. Formato: `U G O` (Usuario, Grupo, Otros), cada uno con valores de `0` a `7`.
- `r`: (Opcional) Aplica los cambios recursivamente a todos los archivos y carpetas dentro de la ruta especificada.

**Ejemplo**:

```bash
chmod >path="/home/user/docs" >ugo=764 >r
```

### PAUSE

Pone la aplicación en pausa hasta que se presione una tecla.

**Sintaxis**:

```bash
pause
```

### REP

Genera reportes gráficos utilizando Graphviz.

**Sintaxis**:

```bash
rep >id=<id_partición> >path=<ruta_reporte> >name=<nombre_reporte> [>ruta=<ruta_archivo>]
```

**Parámetros**:

- `id`: (Obligatorio) ID de la partición sobre la que se generará el reporte.
- `path`: (Obligatorio) Ruta donde se guardará el reporte.
- `name`: (Obligatorio) Tipo de reporte a generar. Valores posibles: `mbr`, `disk`, `file`, `ls`.
- `ruta`: (Opcional) Ruta del archivo o carpeta para los reportes `file` y `ls`.

**Ejemplo**:

```bash
rep >id=061Disco1 >path=/home/user/reports/reporte1.jpg >name=mbr
```
