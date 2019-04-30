#include "pm.h"
#include <minix/com.h>
#include <minix/callnr.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <signal.h>    /* needed only because mproc.h needs it */
#include "mproc.h"

/*===========================================================================*
 *				do DISTORT_TIME				     *
 *===========================================================================*/

 int is_forefather(pid_t parent, pid_t son) {
  register struct mproc *actual = find_proc(son);
  while (actual->mp_pid != 1) {
    if(actual->mp_pid == parent) {
      return 1;
    }
    printf("Sprawdzanie przodka: actual_pid=%d\n", actual->mp_pid);
    actual = &mproc[actual->mp_parent];
  }
  return 0;
 }

 int do_distort_time(void)
{
  pid_t mess_pid = m_in.mess_pm_distort_time.pid;
  pid_t next_pid;
  uint8_t find = 0;
  register struct mproc *mess_mp;

  printf("Funckja do_distort: pid=%d, mess_pid=%d, scale=%d\n", mp->mp_pid, mess_pid, m_in.mess_pm_distort_time.scale);

  if ((mess_mp = find_proc(mess_pid)) == NULL) {
    printf("Nieistnieje\n");
    return EINVAL;
  }

  if (mess_mp->mp_pid == mp->mp_pid) {
    printf("Sa takie same\n");

    return EPERM;
  }

  if (is_forefather(mp->mp_pid, mess_pid)) {
    distort[mess_pid].multi = 1;
    printf("pid=%d : To jest moj syn mess_pid=%d\n", mp->mp_pid, mess_pid);
  } else if (is_forefather(mess_pid, mp->mp_pid)) {
    distort[mess_pid].multi = 0;
    printf("pid=%d : To jest moj ojciec mess_pid=%d\n", mp->mp_pid, mess_pid);
  } else {
    printf("Nie sa spokrewnione\n");
    return EPERM;
  }

  distort[mp->mp_pid].scale = m_in.mess_pm_distort_time.scale;
  distort[mp->mp_pid].base = 0;

  printf("Hacker Rychh!\n");
  return (OK);
}
