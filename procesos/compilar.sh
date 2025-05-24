#!/bin/bash

#Archivos base
BASE="proceso.c"

#Iteramos sobre todas las crpetas en el directorio actual
for DIR in */; do
  #Verificamos que sea un directorio
  if [ -d "$DIR" ]; then
    #buscamos el archivo .c en el directorio
    FILE=$(find "$DIR" -maxdepth 1 -name "*.c" | head -n 1)

    #Verificamos que se encontro un archivo .c
    if [ -n "$FILE" ]; then
      #Obtenemos el nombre que tendra el ejecutable
      EXECUTABLE=$(basename "$FILE" .c)

      #Compilamos
      gcc "$FILE" "$BASE" -o "$EXECUTABLE"

      #Verificamos que no hubo error al compilar
      if [ $? -eq 0 ]; then
	echo "Compilado: $EXECUTABLE"
      else
	echo "Error al compilar: $EXECUTABLE"
      fi
    fi 
  fi 
done 


