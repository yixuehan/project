package main

import "fmt"
import "runtime"
import "time"

func  run(i,n int64 ,ch chan int64){
    count := int64(0)
    for i := int64(i); i < n; i++ {
        count = count +i
    }
    ch <- count

}

func main(){

    t1 := time.Now()
    NCPU := runtime.NumCPU()
    fmt.Printf("NCPU:%d\n",NCPU)
    runtime.GOMAXPROCS(NCPU)
    chs := make([] chan int64,NCPU)
    for i :=int64(0); i < int64(NCPU); i++{
        chs[i] = make(chan int64)
        n := int64(9000000000)/int64(NCPU)
        go run(i*n, (i+1)*n,chs[i])
    }

    count := int64(0)
    for i :=0; i < NCPU;i++{
        t := <- chs[i]
        count = count +t
    }
    t2 := time.Now()

    fmt.Printf("cpu num:%d,cost:%d,count:%d\n",NCPU,t2.Sub(t1)/1000000000,count)
}
