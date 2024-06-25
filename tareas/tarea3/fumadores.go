package main

// Esto es como importar bibliotecas
import (
	"fmt"
	"math/rand"
	"sync"
	"time"
) // End import

// Este es como el enum, iota empieza en 0, para enumerar a cada variable que son constantes
const (
    mariguanas = iota
	filtro
	fuego
) // End const
    
// Arreglo donde se guardan los ingredientes para hacer el puro
var ingredientesPuro = []string{"mariguanas", "filtro", "fuego"}

// Sincronizacion
var wg sync.WaitGroup

/*
* En esta funcion se toman 2 de los 3 ingredientes de forma
* aleatoria para ponerlos en la mesa
*/
func proveedor(mesa [3]chan bool, dealer chan bool) {
	for {
		select {
		case <-dealer:
			// buscar dos numeros aleatorios que sean las posiciones del arreglo
			producto1 := rand.Intn(3)
			producto2 := (producto1 + rand.Intn(2) + 1) % 3

			// escoger los ingredientes del arreglo segun las posiciones anteriores
			fmt.Printf("proveedor pone %s y %s en la mesa.\n", ingredientesPuro[producto1], ingredientesPuro[producto2])

            // para ponerlos en la mesa y notificar que ahi estan
			mesa[producto1] <- true
			mesa[producto2] <- true
		} // End select
	} // End for
} // End proveedor

// Aqui se hace el puro para poder fumar un rato
func fumador(tengo int, mesa [3]chan bool, dealer chan bool) {
	defer wg.Done()
	for {
        // hacer el puro para fumar
		select {
		case <-mesa[(tengo+1)%3]:
			select {
			case <-mesa[(tengo+2)%3]:
				fmt.Printf("Fumador con %s empieza a fumar\n", ingredientesPuro[tengo])
                // fumando
				time.Sleep(time.Second) 

				// termina de fumar
				dealer <- true
			default:
				// si no hay ingrediente, tomar otro, esto ayuda a que no hayan bloqueos
				mesa[(tengo+1)%3] <- true
			} // End select
		} // End select
	} // End for
} // Fumador

func main() {
	rand.Seed(time.Now().UnixNano())

	// Canales tipo bool
	mesa := [3]chan bool{make(chan bool, 1), make(chan bool, 1), make(chan bool, 1)}
	dealer := make(chan bool, 1)

	// pase los ingredientes
	dealer <- true

	// pongalos en la mesa
	go proveedor(mesa, dealer)

	// hagalo y fume
	for i := 0; i < 3; i++ {
		wg.Add(1)
		go fumador(i, mesa, dealer)
	} // End for

	// espere a que termine de fumar para poder empezar a hacer otro puro
	wg.Wait()
} // End main
