#include "user.h"

/**
 * Crea un nuevo User con el nombre dado y sin amigos.
 */
User::User(const std::string& name)
  : _name(name)
  , _friends(nullptr)
  , _size(0)
  , _capacity(0)
{
}

/**
 * Agrega un amigo a la lista de amigos de este User.
 * @param name El nombre del amigo a agregar.
 */
void User::add_friend(const std::string& name)
{
  if (_size == _capacity) {
    _capacity = 2 * _capacity + 1;
    std::string* newFriends = new std::string[_capacity];
    for (size_t i = 0; i < _size; ++i) {
      newFriends[i] = _friends[i];
    }
    delete[] _friends;
    _friends = newFriends;
  }
  _friends[_size++] = name;
}

/**
 * Retorna el nombre de este User.
 */
std::string User::get_name() const
{
  return _name;
}

/**
 * Retorna el número de amigos que tiene este User.
 */
size_t User::size() const
{
  return _size;
}

/**
 * Establece el nombre del amigo en el índice dado.
 * @param index El índice del amigo a modificar.
 * @param name El nombre a asignar.
 */
void User::set_friend(size_t index, const std::string& name)
{
  if (index < _size) {
    _friends[index] = name;
  }
}

/** 
 * Destructor: libera la memoria asignada para la lista de amigos.
 */
User::~User()
{
  delete[] _friends;
}

/**
 * Constructor de copia: realiza una copia profunda del objeto.
 */
User::User(const User& user)
  : _name(user._name)
  , _size(user._size)
  , _capacity(user._capacity)
{
  if (_capacity > 0) {
    _friends = new std::string[_capacity];
    for (size_t i = 0; i < _size; ++i) {
      _friends[i] = user._friends[i];
    }
  } else {
    _friends = nullptr;
  }
}

/**
 * Operador de asignación por copia: realiza una copia profunda.
 */
User& User::operator=(const User& user)
{
  if (this == &user)
    return *this;

  // Libera la memoria actual
  delete[] _friends;

  // Copia los datos del objeto fuente
  _name = user._name;
  _size = user._size;
  _capacity = user._capacity;
  if (_capacity > 0) {
    _friends = new std::string[_capacity];
    for (size_t i = 0; i < _size; ++i) {
      _friends[i] = user._friends[i];
    }
  } else {
    _friends = nullptr;
  }
  return *this;
}

/**
 * Sobrecarga del operador += para agregar un amigo de forma simétrica.
 * Agrega a "rhs" el nombre de este usuario y a este usuario el nombre de "rhs".
 */
User& User::operator+=(User& rhs)
{
  // Agregar a este usuario el nombre de rhs
  this->add_friend(rhs.get_name());
  // Agregar a rhs el nombre de este usuario
  rhs.add_friend(this->get_name());
  return *this;
}

/**
 * Sobrecarga del operador < para comparar usuarios alfabéticamente por nombre.
 */
bool User::operator<(const User& rhs) const
{
  return _name < rhs._name;
}

/**
 * Sobrecarga del operador << para imprimir la información del usuario.
 * Formato esperado: User(name=Alice, friends=[Bob, Charlie])
 */
std::ostream& operator<<(std::ostream& os, const User& user)
{
  os << "User(name=" << user._name << ", friends=[";
  for (size_t i = 0; i < user._size; ++i) {
    os << user._friends[i];
    if (i != user._size - 1) {
      os << ", ";
    }
  }
  os << "])";
  return os;
}
