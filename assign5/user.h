/*
 * CS106L Assignment 5: TreeBook
 * Created by Fabio Ibanez with modifications by Jacob Roberts-Baca.
 */

 #ifndef USER_H
 #define USER_H
 
 #include <iostream>
 #include <string>
 
 class User
 {
 public:
   // Constructor con nombre
   User(const std::string& name);
 
   // Función para agregar un amigo (por nombre)
   void add_friend(const std::string& name);
 
   // Métodos de acceso
   std::string get_name() const;
   size_t size() const;
   void set_friend(size_t index, const std::string& name);
 
   // --- Funciones miembro especiales ---
 
   // Destructor: libera la memoria dinámica
   ~User();
 
   // Constructor de copia: realiza copia profunda
   User(const User& user);
 
   // Operador de asignación por copia: realiza copia profunda
   User& operator=(const User& user);
 
   // Elimina el constructor de movimiento
   User(User&& user) = delete;
 
   // Elimina el operador de asignación por movimiento
   User& operator=(User&& user) = delete;
 
   // --- Sobrecarga de operadores customizados ---
 
   // Sobrecarga del operador += para agregar amigos simétricamente.
   // Se agrega a "rhs" el nombre de este usuario y viceversa.
   User& operator+=(User& rhs);
 
   // Sobrecarga del operador < para comparar usuarios alfabéticamente por nombre.
   bool operator<(const User& rhs) const;
 
   // Declaración de operador << para impresión. Se declara como friend para acceder a miembros privados.
   friend std::ostream& operator<<(std::ostream& os, const User& user);
 
 private:
   std::string _name;
   std::string* _friends;
   size_t _size;
   size_t _capacity;
 };
 
 #endif // USER_H
 