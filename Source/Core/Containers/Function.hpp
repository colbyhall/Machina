/**
 * Copyright (c) 2024-2025 Colby Hall <me@cobeh.com>
 *
 * This software is released under the MIT License.
 */

#pragma once

#include <Core/Memory.hpp>

namespace Forge::Core {
	namespace hidden {
		template <typename Functor, typename FuncType>
		struct FunctionRefCaller;

		template <typename Functor, typename R, typename... P>
		struct FunctionRefCaller<Functor, R(P...)> {
			static R call(void* obj, P&... params) {
				return Core::invoke(*(Functor*)obj, Forge::forward<P>(params)...);
			}
		};

		template <typename Functor, typename... P>
		struct FunctionRefCaller<Functor, void(P...)> {
			static void call(void* obj, P&... params) { Core::invoke(*(Functor*)obj, Forge::forward<P>(params)...); }
		};

		template <typename S, typename F>
		class FunctionBase;

		template <typename S, typename R, typename... Param>
		class FunctionBase<S, R(Param...)> {
		public:
			using Result = R;

			FunctionBase() = delete;

			template <typename F>
			FunctionBase(F&& f)
				requires(!Forge::Core::is_same<FunctionBase, Forge::Core::Decay<F>>)
			{
				if (auto* binding = m_storage.bind(Forge::forward<F>(f))) {
					FORGE_UNUSED(binding);
					using DecayedFunctor = RemovePointer<decltype(binding)>;
					m_callable = &FunctionRefCaller<DecayedFunctor, R(Param...)>::call;
				}
			}
			FunctionBase(const FunctionBase& copy) noexcept = delete;
			FunctionBase& operator=(const FunctionBase& copy) noexcept = delete;
			FunctionBase(FunctionBase&& move) noexcept
				: m_callable(move.m_callable)
				, m_storage(Forge::move(move.m_storage)) {
				move.m_callable = nullptr;
			}
			FunctionBase& operator=(FunctionBase&& move) noexcept {
				this->~FunctionBase();

				m_callable = Forge::move(move.m_callable);
				move.m_callable = nullptr;

				m_storage = Forge::move(move.m_storage);
			}

			R operator()(Param... params) const {
				FORGE_ASSERT(m_callable != nullptr);
				return (m_callable)(m_storage.ptr(), params...);
			}

		private:
			template <typename OtherS, typename OtherF>
			friend class FunctionBase;

			R (*m_callable)(void*, Param&...);
			S m_storage;
		};

		struct RefStorage {
			RefStorage() = default;

			RefStorage(const RefStorage& copy) noexcept = delete;
			RefStorage& operator=(const RefStorage& copy) noexcept = delete;
			RefStorage(RefStorage&& move) noexcept : m_ptr(move.m_ptr) { move.m_ptr = nullptr; }
			RefStorage& operator=(RefStorage&& move) noexcept {
				m_ptr = move.m_ptr;
				move.m_ptr = nullptr;
				return *this;
			}

			template <typename F>
			RemoveReference<F>* bind(F&& f) {
				m_ptr = (void*)&f;
				return &f;
			}
			void* ptr() const { return m_ptr; }

		private:
			void* m_ptr = nullptr;
		};

		struct FunctorWrapperBase {
			virtual void* ptr() = 0;
			virtual ~FunctorWrapperBase() = default;
		};

		template <typename F>
		struct FunctorWrapper final : public FunctorWrapperBase {
			template <typename... Args>
			explicit FunctorWrapper(Args&&... args)
				requires is_constructible<F, Args...>
				: f{ Forge::forward<Args>(args)... } {}
			~FunctorWrapper() final = default;
			FunctorWrapper(const FunctorWrapper& copy) = delete;
			FunctorWrapper& operator=(const FunctorWrapper& copy) = delete;
			FunctorWrapper(FunctorWrapper&& m) : f(Forge::move(m.f)) {}
			FunctorWrapper& operator=(FunctorWrapper&& m) {
				this->~FunctionWrapper();
				f = Forge::move(m.f);
				return *this;
			}

			void* ptr() override { return &f; }

			F f;
		};

		struct UniqueStorage {
			UniqueStorage() = default;
			UniqueStorage(const UniqueStorage& copy) = delete;
			UniqueStorage& operator=(const UniqueStorage& copy) = delete;

			UniqueStorage(UniqueStorage&& s) noexcept : m_ptr(s.m_ptr) { s.m_ptr = nullptr; }
			UniqueStorage& operator=(UniqueStorage&& s) noexcept {
				// m_ptr->~FunctorWrapperBase();

				m_ptr = s.m_ptr;
				s.m_ptr = nullptr;

				return *this;
			}
			~UniqueStorage() {
				if (m_ptr) {
					auto* f = static_cast<FunctorWrapperBase*>(m_ptr);
					f->~FunctorWrapperBase();
					Memory::free(m_ptr);
					m_ptr = nullptr;
				}
			}

			template <typename F>
			RemoveReference<F>* bind(F&& f) {
				void* memory = Memory::alloc(Memory::Layout::single<FunctorWrapper<F>>());
				m_ptr = Memory::emplace<FunctorWrapper<F>>(memory, Forge::forward<F>(f));

				return static_cast<RemoveReference<F>*>(m_ptr->ptr());
			}
			void* ptr() const { return m_ptr->ptr(); }

		private:
			FunctorWrapperBase* m_ptr = nullptr;
		};

		template <typename T>
		T&& declval();

		template <typename F, typename R, typename... P>
		inline constexpr bool functor_return_type_is_compatible =
			Forge::Core::is_constructible<R, decltype(declval<F>()(declval<P>()...))>;
		template <typename MR, typename Class, typename R, typename... P>
		inline constexpr bool functor_return_type_is_compatible<MR Class::*, R, P...> =
			Forge::Core::is_constructible<R, MR>;
		template <typename MR, typename Class, typename R, typename... P>
		inline constexpr bool functor_return_type_is_compatible<MR Class::* const, R, P...> =
			Forge::Core::is_constructible<R, MR>;
		template <typename MR, typename Class, typename... MP, typename R, typename... P>
		inline constexpr bool functor_return_type_is_compatible<MR (Class::*)(MP...), R, P...> =
			Forge::Core::is_constructible<R, MR>;
		template <typename MR, typename Class, typename... MP, typename R, typename... P>
		inline constexpr bool functor_return_type_is_compatible<MR (Class::*)(MP...) const, R, P...> =
			Forge::Core::is_constructible<R, MR>;

		template <typename A, typename B>
		inline constexpr bool func_can_bind_to_functor = false;

		template <typename F, typename R, typename... P>
		inline constexpr bool func_can_bind_to_functor<R(P...), F> =
			Forge::Core::is_invocable<F, P...> && functor_return_type_is_compatible<F, R, P...>;

		template <typename F, typename... P>
		inline constexpr bool func_can_bind_to_functor<void(P...), F> = Forge::Core::is_invocable<F, P...>;
	} // namespace hidden

	template <typename F>
	class Function;

	template <typename T>
	inline constexpr bool is_op_function = false;

	template <typename T>
	inline constexpr bool is_op_function<Function<T>> = true;

	template <typename F>
	class FunctionRef;

	template <typename T>
	inline constexpr bool is_function_ref = false;
	template <typename T>
	inline constexpr bool is_function_ref<FunctionRef<T>> = true;

	template <typename F>
	class FunctionRef final : public hidden::FunctionBase<hidden::RefStorage, F> {
		using Super = hidden::FunctionBase<hidden::RefStorage, F>;

	public:
		using Result = typename Super::Result;

		template <typename Functor>
			requires(!is_function_ref<Decay<Functor>> && hidden::func_can_bind_to_functor<F, Decay<Functor>>)
		FunctionRef(Functor&& f) : Super(Forge::forward<Functor>(f)) {}
		FunctionRef(const FunctionRef& copy) noexcept = delete;
		FunctionRef& operator=(const FunctionRef& copy) const noexcept = delete;
		FunctionRef(FunctionRef&& move) noexcept = default;
		FunctionRef& operator=(FunctionRef&& move) noexcept = default;
		~FunctionRef() = default;
	};

	template <typename F>
	class Function final : public hidden::FunctionBase<hidden::UniqueStorage, F> {
		using Super = hidden::FunctionBase<hidden::UniqueStorage, F>;

	public:
		using Result = typename Super::Result;

		template <typename Functor>
			requires(!is_op_function<Decay<Functor>> && hidden::func_can_bind_to_functor<F, Decay<Functor>>)
		Function(Functor&& f) : Super{ Forge::forward<Functor>(f) } {}
		Function(const Function& copy) noexcept = delete;
		Function& operator=(const Function& copy) noexcept = delete;
		Function(Function&& move) noexcept = default;
		Function& operator=(Function&& move) noexcept = default;
		~Function() = default;
	};
} // namespace Forge::Core

namespace Forge {
	using Core::Function;
	using Core::FunctionRef;
} // namespace Forge
