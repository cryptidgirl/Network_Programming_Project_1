#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//should have client prototype defined at the top
//should have any helper program prototypes defined at the top
//should not be using DieWithError since this application should remain running
//should handle errors without dying as much as possible

int main(void) {

  //--------------------------------------------
  // User interface application 
  // This application will run all of the time and 
  // when needed call the Client application 
  // the Application should have no knowledge of the 
  // server. i
  // Application simply manages the user input and requests and returns responses
  // back to the user from the "client"; the client needs to manage application requests
  // and then work with the server using a defined protocol between client and server
  // 
  // you will need to define a protocol for communication between client and server
  // by protocol I do not mean TCP/UDP but instead what steps and what data is sent so 
  // that the server knows what is being requested and how to respond
  // also so that when the server does respond the client understands how to manage it
  // so that the proper response can be diosplayed by the application to the user
  //--------------------------------------------
    int userChoice;
    system("./Server.o");
    int Client(int operationType, int operationSubtype, char* userInput);

  for (;;) {
     // application requires main menu and sub menus to capture input correctly after each user interaction
     // control should be returned to the main menu for next user input
      printf("Choose one (Enter a number):\n ");
      printf("1: Get Server Time\n 2: Get the Quote of the Day\n 3:Country Info\n");
      scanf("%d", &userChoice);
      
     // User input options: 
     // 1. Get the server time in one of 2 formats
     //    either GMT or local time and date (user gets option on which format)
     //      1.a GMT
     //      1.b Local
     //
      if(userChoice == 1){
          system("@cls||clear");
          printf("Choose a time format (Enter a number):\n");
          printf("1: GMT\n2: Local\n");
          scanf("%d", &userChoice);
          if(userChoice == 1){
              Client(1, 1, ".");
          }
          else if(userChoice == 2){
              Client(1, 2, ".");
          } else {
              printf("Choose 1 or 2");
          }
          
      }
     // 2. Request a Quote of the Day to brighten their day
     //    (request and response should be encrypted across the wire 
     //     but the user and the "application" doesnt need to know this)
     //      2.a if the user doesnt like the quote they get to "unlike" it
     //      2.b if the user likes the quote they get to "like" it
     //      2.c user may choose to do neither
     //
     // 3. Request one of the following based upon a country ID: 
     //      Enter Id to get Country Code
     //      Enter Id to get Country Name
     //      Enter Id to get Continent the Country is associated with if applicable
     //      (again request response should be encrypted across the wire 
     //       but the user and the "application" doesnt need to know this)
     //       3.a Country Code lookup
     //       3.b Country Name lookup
     //       3.c Country Continent Lookup
     //  
     //   Errors that must be dealt with by the Application: 
     //      - no date time returned;
     //      - country information not returned;
     //      - some of the countries dont have a continent - appropriately display that to user
     //      - user doesnt like/unlike the quote - need to return something to server, release client and
     //        return the Application to main menu

  }

return 0;
}
