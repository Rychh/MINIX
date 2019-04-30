#include <lib.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <minix/rs.h>

int get_pm_endpt(endpoint_t *pt) {
  return minix_rs_lookup("pm", pt);
}

int distort_time(pid_t pid, uint8_t scale) {
  endpoint_t pm_pt;
  message m;
  printf("Funckja Bilioteczna:");// pid=%d, scale= %d", pid, scale);

  if (get_pm_endpt(&pm_pt) != 0) {
    printf("no oc chodzi?");
    errno = ENOSYS;
    return -1;
  }


  m.mess_pm_distort_time.pid = pid;
  m.mess_pm_distort_time.scale = scale;

  if (_syscall(pm_pt, PM_DISTORT_TIME, &m))
    return errno;
  return 0;
}