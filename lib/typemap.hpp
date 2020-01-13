#pragma once

#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <memory>
#include <optional>
#include <type_traits>

namespace type {

using DeleterFn = void (*)(void *);

/**
 * Simple deleter, that allows to delete void pointer with provided type.
 *
 * Safe only if you can guarantee that void* is actually T*
 */
template<typename T>
void type_deleter(void* val) {
    //Deleter is not invoked if pointer is null
    delete static_cast<T*>(val);
}

/**
 * Creates type erased unique_ptr, by storing exact type in custom deleter
 */
template <typename T, class... Args>
inline std::unique_ptr<void, DeleterFn> make_dyn_ptr(Args &&... args) {
  return std::unique_ptr<T, DeleterFn>(new T(std::forward<Args>(args)...), type_deleter<T>);
}

/**
 * Type-safe map, that uses type as key.
 */
class Map {
    std::unordered_map<std::type_index, std::unique_ptr<void, void (*)(void*)>> storage;

    public:
        Map() = default;

    /**
     * Retrieves element from map, inserting default value if no value is present.
     */
    template<class T>
    T& get_or_default() {
        static_assert(std::is_default_constructible<T>::value, "T lacks default constructor");

        //Seems like default insertion is not available for pointers :(
        auto search = this->storage.find(std::type_index(typeid(T)));

        if (search == this->storage.end()) {
            auto res = this->storage.emplace(std::make_pair(std::type_index(typeid(T)), make_dyn_ptr<T>()));
            return *(static_cast<T*>(res.first->second.get()));
        } else {
            return *(static_cast<T*>(search->second.get()));
        }
    }

    /**
     * Retrieves element from map
     *
     * @note std::optional<T&> is not supported, so don't use it.
     *
     * @returns Pointer to the value
     */
    template<class T>
    T* get() {
        auto search = this->storage.find(std::type_index(typeid(T)));
        if (search == this->storage.end()) {
            return nullptr;
        } else {
            return static_cast<T*>(search->second.get());
        }
    }

    /**
     * Retrieves element from map
     *
     * @note std::optional<T&> is not supported, so don't use it.
     *
     * @returns Pointer to the value
     */
    template<class T>
    const T* get() const {
        return const_cast<Map*>(this)->get<T>();
    }

    /**
     * Checks if element is present in map
     */
    template<class T>
    bool has() const {
        auto search = this->storage.find(std::type_index(typeid(T)));
        return search != this->storage.end();
    }

    /**
     * Inserts element, returning previous one, if any
     */
    template<class T, class... Args>
    std::optional<T> emplace(Args&&... args) {
        static_assert(std::is_constructible<T, Args...>::value, "T cannot be constructed with provided arguments");

        auto search = this->storage.find(std::type_index(typeid(T)));

        if (search == this->storage.end()) {
            this->storage.emplace(std::make_pair(std::type_index(typeid(T)), make_dyn_ptr<T>(std::forward<Args>(args)...)));
            return {};
        } else {
            std::optional<T> value;
            value.emplace(std::forward<Args>(args)...);

            std::swap(*value, *static_cast<T*>(search->second.get()));
            return value;
        }
    }

    /**
     * Removes element, returning already inserted value, if any
     */
    template<class T>
    std::optional<T> remove() {
        static_assert(std::is_move_constructible<T>::value, "T cannot be moved out");

        auto search = this->storage.find(std::type_index(typeid(T)));

        if (search == this->storage.end()) {
            return {};
        } else {
            std::optional<T> value(std::move(*static_cast<T*>(search->second.get())));
            this->storage.erase(search);
            return value;
        }
    }

    /**
     * Clears map.
     */
    void clear() noexcept {
        this->storage.clear();
    }


    /**
     * Checks whether map is empty
     */
    bool empty() noexcept {
        return this->storage.empty();
    }
};

}
