// operaciones.h
#pragma once
#include <string>
#include <mysql.h>
#include "ConexionDB.h"

// Libros
void consultarLibros(MYSQL* con);
void consultarLibroPorId(MYSQL* con);
void agregarLibro(MYSQL* con);

// Préstamos
void consultarPrestamosUsuario(MYSQL* con, const std::string& userCedula);
void hacerPrestamo(MYSQL* con, const std::string& userCedula);
void devolverPrestamo(MYSQL* con, const std::string& userCedula);
void verTodosLosPrestamos(MYSQL* con);
bool registrarUsuario(ConexionBD& cn);
bool resetPassword(ConexionBD& cn);
void listarPreguntas(ConexionBD& cn);
bool registrarPregunta(ConexionBD& cn, const std::string& cedula);
bool validarPregunta(ConexionBD& cn, const std::string& cedula);
void verPreguntaSeguridad(ConexionBD& cn, const std::string& userId);
void listarCategorias(MYSQL* con);
void asignarCategorias(MYSQL* con, unsigned long libroId);
void filtrarPorCategoria(MYSQL* con);
std::string seleccionarIdioma(MYSQL* con);
int seleccionarCategoria(MYSQL* con);
std::string seleccionarOrden();
void filtrarLibros(MYSQL* con);
void listarPorIdioma(MYSQL* con);
void listarLibrosPorCategoria(MYSQL* con);
// … más arriba …
void buscarLibrosPorNombre(MYSQL* con);
void buscarEstudianteYPrestamos(MYSQL* con);


