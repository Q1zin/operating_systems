use libc::{c_int, c_void, pid_t};

#[repr(C)]
pub struct MyThread {
    _private: [u8; 0],
}

#[allow(non_camel_case_types)]
pub type mythread_t = *mut MyThread;

#[allow(non_camel_case_types)]
pub type start_routine_t = unsafe extern "C" fn(arg: *mut c_void, thread: mythread_t) -> *mut c_void;

#[allow(dead_code)]
unsafe extern "C" {
    pub fn mythread_create(
        tid: *mut mythread_t,
        routine: start_routine_t,
        arg: *mut c_void,
    ) -> c_int;

    pub fn mythread_self() -> pid_t;

    pub fn mythread_join(tid: mythread_t, retval: *mut *mut c_void) -> c_int;

    pub fn mythread_cancel(tid: mythread_t);

    pub fn mythread_testcancel(thread: mythread_t);

    pub fn mythread_detach(tid: mythread_t) -> c_int;
}