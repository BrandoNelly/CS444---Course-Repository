# Brandon Nelson -- Project 2: reservations

## Building

Command line:

* `make` to build. An executable called `reservations` will be produced.

## Files


* `reservations.c`: The main (and only) file of the program.
* `Makefile`: used to compile the program to obtain executable.

## Data

* Integer representation of:
    broker counts,
    seats taken count,
    an array of seats and if they are taken (o = not taken, 1 = taken),
    transaction counts

* pthread mutual exclusion for locking and unlocking data at critical sections of the code


## Functions

* `main()`Executes the main function of the program. Prints the seat broker work results.
  * `reserve_seat()`: Checks that a seat is not reserved and if it is not, then reserves it
  * `free_seat()`: Checks that a seat is reserved and if it is so, then unreserves it
  * `is_free()`: Returns true if the seat is unreserved
  * `verify_seat_count()`: Counts all of the taken seats in the seats_taken array and compares the total count with the local count, returning true if they are the same
  * `seat_broker()`: Handles the random reserving and freeing of seats for the transaction count given and calling the `verify_seat_count()` function to confirm work

## Notes

* Running the command without any argument shows help:
    % ./reservations
    usage: reservations seat_count broker_count xaction_count

* Play around with the quantities for brokers, seats and transactions made. The problem should work out nicely!