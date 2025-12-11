use std::ptr;
use libc::c_void;

mod ffi;
use ffi::*;

unsafe extern "C" fn thread_func(arg: *mut c_void, _thread: mythread_t) -> *mut c_void {
    let value = arg as *mut i32;
    unsafe {
        println!("Hello from mythread!!! FROM RUST!!! arg = {}", *value);
    }
    ptr::null_mut()
}

fn main() {
    unsafe {
        let mut tid: mythread_t = ptr::null_mut();
        let mut arg: i32 = 42;

        let res = mythread_create(
            &mut tid as *mut mythread_t,
            thread_func,
            &mut arg as *mut _ as *mut c_void,
        );
        
        if res != 0 {
            eprintln!("mythread_create failed: {}", res);
            return;
        }

        let mut retval: *mut c_void = ptr::null_mut();
        let res = mythread_join(tid, &mut retval as *mut *mut c_void);
        if res != 0 {
            eprintln!("mythread_join failed: {}", res);
        }
    }
}