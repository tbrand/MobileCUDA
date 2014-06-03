Mobile CUDA

Mobile CUDA is a runtime library witch does "Managing Multiple CUDA Processes Dynamicaly for Hyper-Q".
Support CUDA-5.5.

*Utilize

Since the library is runtime one, you have to recompile your application with the flag -lcudart like as follows

>nvcc app.cu -lcudart

Run a deamon witch is located in /path for Mobile CUDA/daemon/mocu_daemon

>./mocu_daemon

Open a library path like this

>export LD_LIBRARY_PATH=/path for Mobile CUDA/lib64/

Try to run some CUDA application.


Hyper-Q is a hardware function on NVIDIA's GPU (supported Kepler and above gen) witch can run multiple applications on single device efficiently.
If you have oppotunity for running multiple CUDA applications on single node, this project is very helpfull for you.

author Taichirou Suzuki.

