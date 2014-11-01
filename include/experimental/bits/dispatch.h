//
// dispatch.h
// ~~~~~~~~~~
// Schedule a function to run now if possible, otherwise later.
//
// Copyright (c) 2014 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef EXECUTORS_EXPERIMENTAL_BITS_DISPATCH_H
#define EXECUTORS_EXPERIMENTAL_BITS_DISPATCH_H

#include <experimental/bits/work_dispatcher.h>

namespace std {
namespace experimental {
inline namespace concurrency_v1 {

template <class _CompletionToken>
auto dispatch(_CompletionToken&& __token)
{
  async_completion<_CompletionToken, void()> __completion(__token);
  auto __completion_executor(get_associated_executor(__completion.handler));
  auto __completion_allocator(get_associated_allocator(__completion.handler));
  __completion_executor.dispatch(std::move(__completion.handler), __completion_allocator);
  return __completion.result.get();
}

template <class _Executor, class _CompletionToken>
auto dispatch(const _Executor& __e, _CompletionToken&& __token,
  typename enable_if<is_executor<_Executor>::value>::type*)
{
  typedef typename handler_type<_CompletionToken, void()>::type _Handler;
  async_completion<_CompletionToken, void()> __completion(__token);
  _Executor __completion_executor(__e);
  auto __completion_allocator(get_associated_allocator(__completion.handler));
  __completion_executor.dispatch(__work_dispatcher<_Handler>(__completion.handler), __completion_allocator);
  return __completion.result.get();
}

template <class _ExecutionContext, class _CompletionToken>
inline auto dispatch(_ExecutionContext& __c, _CompletionToken&& __token,
  typename enable_if<is_convertible<
    _ExecutionContext&, execution_context&>::value>::type*)
{
  return (dispatch)(__c.get_executor(), forward<_CompletionToken>(__token));
}

} // inline namespace concurrency_v1
} // namespace experimental
} // namespace std

#endif
