package DataService;

import homeParameters.HomeParameters;

import java.util.Scanner;

public class DataService {

    public static void inputData() {

        //INPUT
        Scanner scanner = new Scanner(System.in);

        System.out.print("Podaj liczbę pokoi: ");
        int roomsNumber = scanner.nextInt();

        System.out.print("Podaj liczbę okien: ");
        int windowsNumber = scanner.nextInt();

        System.out.print("Podaj liczbę drzwi wejściowych: ");
        int doorsNumber = scanner.nextInt();

        int [] roomArea = new int[roomsNumber];

        for (int i = 0; i < roomsNumber; i++) {
            System.out.print("Podaj powierzchnię " + (i + 1) +" pokoju: ");
            roomArea[i] = scanner.nextInt();
        }


        //OUTPUT
        HomeParameters home = new HomeParameters(roomsNumber, roomArea, windowsNumber, doorsNumber);
        System.out.println("Czujniki antypożarowe: " + home.fireProtectionModule());
        int [] tab = home.theftProtectionModule();
        System.out.println("Czujniki antywłamaniowe HC: " + tab[0]);
        System.out.println("Czujniki antywłamaniowe PIR: " + tab[1]);
    }


}
