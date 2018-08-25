// ============================================================================
// Copyright (c) 2017-2018, by Vitaly Grigoriev, <Vit.link420@gmail.com>.
// This file is part of ProtocolAnalyzer open source project under MIT License.
// ============================================================================

#ifndef PROTOCOL_ANALYZER_FRAMEWORK_CALLBACK_FUNCTORS_HPP
#define PROTOCOL_ANALYZER_FRAMEWORK_CALLBACK_FUNCTORS_HPP

#include <cstddef>  // std::size_t.
#include <cstdint>  // uint16_t.


namespace analyzer::framework::callbacks
{
    /**
     * @class BaseCallbackFunctor   FrameworkCallbackFunctors.hpp   "include/framework/FrameworkCallbackFunctors.hpp"
     * @brief Base class of framework callback functors.
     */
    class BaseCallbackFunctor
    {
    public:
        BaseCallbackFunctor(void) = default;
        virtual ~BaseCallbackFunctor(void) = default;

        BaseCallbackFunctor (BaseCallbackFunctor &&) = delete;
        BaseCallbackFunctor (const BaseCallbackFunctor &) = delete;
        BaseCallbackFunctor & operator= (BaseCallbackFunctor &&) = delete;
        BaseCallbackFunctor & operator= (const BaseCallbackFunctor &) = delete;
    };


    /**************************************** MODULE_SOCKET ****************************************/

    /**
     * @enum MODULE_SOCKET_TYPES
     * @brief Type of modules in protocol analyzer framework.
     */
    enum MODULE_SOCKET_TYPES : uint16_t
    {
        MODULE_SOCKET_BEFORE_SEND = 0,
        MODULE_SOCKET_AFTER_RECEIVE,
        MODULE_SOCKET_TYPES_SIZE
    };

    /**
     * @class SocketCallbackFunctorBeforeSend   FrameworkCallbackFunctors.hpp   "include/framework/FrameworkCallbackFunctors.hpp"
     * @brief This interface class-functor declares the functor of MODULE_SOCKET_BEFORE_SEND type.
     */
    class SocketCallbackFunctorBeforeSend : public BaseCallbackFunctor
    {
    public:
        SocketCallbackFunctorBeforeSend(void) = default;
        ~SocketCallbackFunctorBeforeSend(void) = default;

        /**
         * @fn virtual void SocketCallbackFunctorBeforeSend::operator() (char *, std::size_t *) const noexcept;
         * @brief Function call operator for SocketCallbackFunctorBeforeSend class-functor.
         * @param [in] data - Pointer to the raw data.
         * @param [in] length - Pointer to the length of the inputted raw data.
         */
        virtual void operator() (char * /*data*/, std::size_t * /*length*/) const noexcept = 0;
    };

    /**
     * @class SocketCallbackFunctorAfterReceive   FrameworkCallbackFunctors.hpp   "include/framework/FrameworkCallbackFunctors.hpp"
     * @brief This interface class-functor declares the functor of MODULE_SOCKET_AFTER_RECEIVE type.
     *
     * @note This functor can be called several times for calling a function. For example: Socket::Recv with CompleteFunctor.
     */
    class SocketCallbackFunctorAfterReceive : public BaseCallbackFunctor
    {
    public:
        SocketCallbackFunctorAfterReceive(void) = default;
        ~SocketCallbackFunctorAfterReceive(void) = default;

        /**
         * @fn virtual void SocketCallbackFunctorBeforeSend::operator() (const char *, const std::size_t) const noexcept;
         * @brief Function call operator for SocketCallbackFunctorAfterReceive class-functor.
         * @param [in] data - Pointer to the constant raw data.
         * @param [in] length - Length of the inputted raw data.
         */
        virtual void operator() (const char * /*data*/, const std::size_t /*length*/) const noexcept = 0;
    };

    /**************************************** MODULE_SOCKET ****************************************/

}  // namespace callbacks.

#endif  // PROTOCOL_ANALYZER_FRAMEWORK_CALLBACK_FUNCTORS_HPP
