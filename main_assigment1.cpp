/*
 * CS106L Assignment 1: SimpleEnroll
 * Created by Fabio Ibanez with modifications by Jacob Roberts-Baca.
 *
 * Welcome to Assignment 1 of CS106L! Please complete each STUDENT TODO
 * in this file. You do not need to modify any other files.
 *
 * Students must implement: +19 , write_courses_offered,
 * write_courses_not_offered
 */

 #include <algorithm>
 #include <fstream>
 #include <iostream>
 #include <sstream>
 #include <string>
 #include <vector>
 
 const std::string COURSES_OFFERED_PATH = "student_output/courses_offered.csv";
 const std::string COURSES_NOT_OFFERED_PATH = "student_output/courses_not_offered.csv";
 
 /**
  * Represents a course a student can take in ExploreCourses.
  *
  * Para esta tarea, un Course tiene:
  * - title: el título del curso (std::string)
  * - number_of_units: el número de unidades (int)
  * - quarter: el trimestre en el que se ofrece (std::string)
  */
 struct Course {
   std::string title;
   int number_of_units;
   std::string quarter;
 
   // Constructor que inicializa el curso a partir de tres strings.
   Course(const std::string &t, const std::string &units, const std::string &q)
       : title(t), number_of_units(std::stoi(units)), quarter(q) {}
 
   // Constructor por defecto.
   Course() : title(""), number_of_units(0), quarter("") {}
 };
 
 /**
  * Nota:
  * Se debe incluir utils.cpp DESPUÉS de declarar la estructura Course, 
  * para que el código de utils.cpp sepa qué es un Course.
  */
 #include "utils.cpp"//NO QUITAR
 
 /**
  * parse_csv: Lee el archivo CSV (courses.csv) y llena el vector de cursos.
  * Se ignora la primera línea (encabezado).
  */
 void parse_csv(const std::string &filename, std::vector<Course> &courses) {
   std::ifstream file(filename);
   if (!file.is_open()) {
     std::cerr << "Error opening file: " << filename << std::endl;
     return;
   }
   
   std::string line;
   // Descartar la línea de encabezado
   std::getline(file, line);
   
   // Leer cada línea (registro)
   while (std::getline(file, line)) {
     if (line.empty())
       continue;
     std::vector<std::string> tokens = split(line, ',');
     if (tokens.size() >= 3) {
       // Se crea el objeto Course usando el constructor (convierte el número de unidades)
       Course course(tokens[0], tokens[1], tokens[2]);
       courses.push_back(course);
     }
   }
   file.close();
 }
 
 /**
  * write_courses_offered:
  * 1) Escribe en "student_output/courses_offered.csv" los cursos cuyo campo quarter NO es "null".
  * 2) Elimina esos cursos del vector all_courses (dejando solo los no ofrecidos).
  */
 void write_courses_offered(std::vector<Course> &all_courses) {
   std::ofstream outfile(COURSES_OFFERED_PATH);
   if (!outfile.is_open()) {
     std::cerr << "Error opening file: " << COURSES_OFFERED_PATH << std::endl;
     return;
   }
   
   // Escribir encabezado
   outfile << "Title,Number of Units,Quarter\n";
   
   std::vector<Course> not_offered;
   for (const auto &course : all_courses) {
     // Un curso se considera ofrecido si su campo quarter NO es "null"
     if (course.quarter != "null") {
       outfile << course.title << "," << course.number_of_units << "," << course.quarter << "\n";
     } else {
       not_offered.push_back(course);
     }
   }
   outfile.close();
   
   // Actualizar all_courses para que contenga solo los cursos no ofrecidos.
   all_courses = std::move(not_offered);
 }
 
 /**
  * write_courses_not_offered:
  * Escribe en "student_output/courses_not_offered.csv" los cursos NO ofrecidos.
  * Se llama siempre después de write_courses_offered, por lo que el vector ya contiene
  * únicamente los cursos no ofrecidos.
  */
 void write_courses_not_offered(const std::vector<Course> &unlisted_courses) {
   std::ofstream outfile(COURSES_NOT_OFFERED_PATH);
   if (!outfile.is_open()) {
     std::cerr << "Error opening file: " << COURSES_NOT_OFFERED_PATH << std::endl;
     return;
   }
   
   // Escribir encabezado
   outfile << "Title,Number of Units,Quarter\n";
   for (const auto &course : unlisted_courses) {
     outfile << course.title << "," << course.number_of_units << "," << course.quarter << "\n";
   }
   outfile.close();
 }
 
 int main() {
   // Verifica que la estructura Course está definida correctamente (usa concept de C++20)
   static_assert(is_valid_course<Course>, "Course struct is not correctly defined!");
   
   std::vector<Course> courses;
   parse_csv("courses.csv", courses);
   
   // Opcional: imprimir cursos para depuración
   // print_courses(courses);
   
   write_courses_offered(courses);
   write_courses_not_offered(courses);
   
   return run_autograder();
 }
 