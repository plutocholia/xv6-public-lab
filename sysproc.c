#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_count_of_digit(void)
{
  int num;
  // if (argint(0, &num) < 0) 
  //   return -1;
  struct proc* my_proc = myproc();
  num = my_proc->tf->ebx;
  // cprintf("test:%d\n", num);
  return count_of_digit(num);
} 

int
sys_print_syscalls(void){
  return print_syscalls();
}

int
sys_handle_alarms(void)
{
  handle_alarms();
  return 0;
}

int
sys_set_alarm(void)
{
  int n;
  argint(0, &n);
  if (n <= 0)
  {
    cprintf("set_alarm argument must be positive.\n");
    return -1;
  }  
  set_alarm(n);
  return 0;
}

int
sys_set_edx(void){
  int num;
  if(argint(0, &num) < 0) return -1;
  set_edx(num);
  return 0;
}

int
sys_read_registers(void){
  read_registers();
  return 0;
}

int
sys_print_proc_info(void) {
  print_proc_info();
  return 0;
}

int
sys_set_proc_tickets(void) {
  int pid, tickets;
  argint(0, &pid);
  argint(1, &tickets);
  set_proc_tickets(pid, tickets);
  return 0;
}

int
sys_set_proc_level(void) {
  int pid, level;
  argint(0, &pid);
  argint(1, &level);
  set_proc_level(pid, level);
  return 0;
}

int
sys_check_lock(void){
  check_lock();
  return 0;
}

int sys_sys_count(void) {
  sys_count();
  return 0;
}

int
sys_shmget(void){
  int page_id;
  if(argint(0, &page_id) < 0)
    return -1;
  return (int)shmget(page_id);
}

int
sys_testshmget(void){
  int mode;
  if(argint(0, &mode) < 0)
    return -1;
  
  cprintf("PID : %d\n", myproc()->pid);
  
  if(mode == 4){ // read for testing parent process after forking new one who creates new share
    int* res2 = shmget(2);
    cprintf("read mode: res2 addr = %x\n",  res2);
    cprintf("read mode: res2 data = %d\n", *res2);
  }
  else if(mode == 3){ // read page 1 and write on page 2 for testing fork in child
    // int* res2 = (int*)0x2000;
    int* res1 = shmget(1);
    cprintf("read mode: res1 addr = %x\n",  res1);
    cprintf("read mode: res1 data = %d\n", *res1);
    int* res2 = shmget(2);
    *res2 = -69;
    cprintf("write mode: res2 addr = %x\n",  res2);
    cprintf("write mode: res2 data = %d\n", *res2);
  }
  else if(mode == 2){ // read
    int* res1 = shmget(1);
    cprintf("read mode: res1 addr = %x\n",  res1);
    cprintf("read mode: res1 data = %d\n", *res1);
  }
  else if(mode == 1){ // write and read
    int* res1 = shmget(1);
    *res1 = 12;
    cprintf("write mode: res1 addr = %x\n",  res1);
    cprintf("write mode: res1 data = %d\n", *res1);
  }
  
  return 0;
}