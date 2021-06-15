
In my project, I can only provide streaming with only one channel and one client. Because of that there is only one way to run. The streaming video will run forever.If you want to stop, press "CTRL+C" from client side.
____________________________________________________________________________

For server side, compile and run c file with these commands:

------------------------------------------------------------
gcc sserver.c -o sserver -lpthread 
------------------------------------------------------------
./sserver –p [port_no] –s 1 –ch1 [file_name]
------------------------------------------------------------

(please dont change channel number)
(give file name without ".txt")
Example run is given below: 

>> gcc sserver.c -o sserver -lpthread 
>> ./sserver –p 5555 –s 1 –ch1 short_intro

_____________________________________________________________________________

For client side, compile and run java file (after running server):

---------------------------------------------------------------------
javac sclient.java
---------------------------------------------------------------------
java sclient –a 127.0.0.1 –p [port_no] –ch 1
---------------------------------------------------------------------

Example run for java client:
>> javac sclient.java
>> java sclient –a 127.0.0.1 –p 5555 –ch 1

The streaming video will be displayed on the terminal (no other external place).
To exit press CTRL+C.
