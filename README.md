# Thread synchronization

## Spinlock adder

### Compile

```bash
$ make sum
```

### Usage

```bash
$ ./sum <numbers> <threads>
```

### Example

```bash
$ ./sum 10000000 256
Sum: 30678
Time: 46 ms
```

## Semaphore producer-consumer

### Compile

```bash
$ make prime
```

### Usage

```bash
$ ./prime <buffer-size> <producer-threads> <consumer-threads>
```

### Example

```bash
$ ./prime 32 1 2
Time: 252 ms
```