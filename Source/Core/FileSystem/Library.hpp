/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Containers/SharedPtr.hpp>
#include <Core/Containers/StringView.hpp>
#include <Core/TypeTraits.hpp>

namespace Forge::Core {
	class Library;

	template <typename T>
	class Symbol;

	template <typename Ret, typename... Args>
	class Symbol<Ret(Args...)> {
	public:
		using FunctionType = Ret(Args...);
		using FunctionPtr = Ret (*)(Args...);

		explicit Symbol() : m_function(nullptr) {}

		/**
		 * Constructs a Symbol from a function pointer.
		 *
		 * @tparam Functor The type of the function pointer.
		 * @param f The function pointer to bind to the Symbol.
		 *
		 * @note The function pointer must be compatible with the Symbol's signature.
		 */
		Symbol(FunctionPtr f, const Library& library);

		FORGE_NO_COPY(Symbol);
		Symbol(Symbol&& move) : m_function(move.m_function), m_library(Forge::move(move.m_library)) {
			move.m_function = nullptr;
		}
		Symbol& operator=(Symbol&& move) {
			this->~Symbol();

			m_function = move.m_function;
			move.m_function = nullptr;
			m_library = Forge::move(move.m_library);

			return *this;
		}

		/**
		 * Calls the function pointer with the provided arguments.
		 *
		 * @param args The arguments to pass to the function.
		 * @return The result of the function call.
		 */
		Ret operator()(Args... args) const {
			FORGE_ASSERT(m_function != nullptr);
			return m_function(Forge::forward<Args>(args)...);
		}

		/**
		 * Checks if the Symbol is valid (i.e., it contains a function pointer).
		 *
		 * @return True if the Symbol is valid, false otherwise.
		 */
		bool is_valid() const { return m_function != nullptr; }

	private:
		FunctionPtr m_function = nullptr;
		Forge::SharedPtr<Library> m_library;
	};

	class Library : public Forge::SharedPtrFromThis<Library> {
	public:
		/**
		 * Loads a library from the specified path.
		 *
		 * @param path The path to the library.
		 * @return A shared pointer to the loaded library. It may be null if the library could not be loaded.
		 */
		static Forge::SharedPtr<Library> load(const StringView& path);

		template <typename T>
		Symbol<T> find_symbol(const StringView& name) const {
			auto* symbol = find_symbol_impl(name);
			if (symbol == nullptr) {
				return Symbol<T>();
			}
			return Symbol<T>(reinterpret_cast<T*>(symbol), *this);
		}

		virtual ~Library() {}

	protected:
		virtual void* find_symbol_impl(const StringView& name) const = 0;
	};

	template <typename Ret, typename... Args>
	Symbol<Ret(Args...)>::Symbol(FunctionPtr f, const Library& library)
		: m_function(f)
		, m_library(library.to_shared()) {
		FORGE_ASSERT(m_function != nullptr);
	}

} // namespace Forge::Core
