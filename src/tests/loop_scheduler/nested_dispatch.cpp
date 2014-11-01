#include <experimental/loop_scheduler>
#include <experimental/executor>
#include <cassert>

int main()
{
  std::experimental::loop_scheduler scheduler;
  auto ex = scheduler.get_executor();

  int count = 0;

  std::experimental::post(ex,
    [&]()
    {
      int count_before_dispatch = count;
      std::experimental::dispatch(scheduler, [&](){ ++count; });
      assert(count == count_before_dispatch + 1);
    });

  int count_before_dispatch = count;
  std::experimental::dispatch(scheduler, [&](){ ++count; });
  assert(count == count_before_dispatch);

  scheduler.run();
  assert(count == 2);
}
