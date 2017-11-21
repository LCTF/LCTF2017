#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include <signal.h>

/*
Books & Audible
Movies, Music & Games
Electronics, Computers & Office
Home, Garden & Tools
Beauty & Health
Toys, Kids & Baby
Clothing, Shoes & Jewelry
Handmade
Sports & Outdoors
Automotive & Industrial
*/
void* cart[99] = {0};
int cart_count = 0;
double cart_total = 0.0;

void get_str(char *buf, int size, char term) {
        char ch;
        int i = 0;

        for (i = 0; i < size; i++) {
                read(0, &ch, 1);
                if (ch == term) {
                        buf[i] = '\0';
                        return;
                } else {
                        buf[i] = ch;
                }
        }

        if (i == size) {
                buf[size-1] = '\0';
        } else {
                buf[i] = '\0';
        }
}

unsigned get_choice() {
        char buf[20];
        get_str(buf, 20, '\n');
        return (unsigned)atoi(buf);
}

unsigned char get_char() {
        char buf[20];
        get_str(buf, 20, '\n');
        return buf[0];
}

char* get_message(int size)
{
	char *message;
	int i = 0;

	if (size==0)
	{
		message = malloc(0x10);

		while (1)
		{
			message[i] = getchar();

			if (message[i] == '\n')
			{
				message[i] = '\0';
				break;
			}

			if (i++%8==0)
			{
				realloc(message, i+8);
			}
		}
	}
	else
	{
		message = malloc(size);
		get_str(message, size, '\n');
	}

	return message;
}

int check_cart(void *item_t)
{
	for (int i = 0; i < 99; ++i)
	{
		if (cart[i]==0)
		{
			cart[i] = item_t;
			cart_count++;
			return 1;
		}
	}

	printf("No more space!\n");
	return 0;
}

struct _item_a
{
  char mark[32];
  char name[32];
  char writer[32];
  char padding[96];
  double price;
  char *message;
  void (*print)();
  int pad;
};

void print_item_a(struct _item_a *item_t)
{
	printf("Name: %s\t\tWriter: %s\t\tPrice: $%.2f\n", item_t->name, item_t->writer, item_t->price);
}

struct _item_a item_a[] = {
	{print:print_item_a, name:"Leonardo da Vinci\0",writer:"Walter Isaacson\0",price:20.99},
	{print:print_item_a, name:"Obama: An Intimate Portrait\0",writer:"Pete Souza and Barack Obama\0",price:28.99},
	{print:print_item_a, name:"Dog Man\0",writer:"Dav Pilkey\0",price:9.99},
	{print:print_item_a, name:"The Getaway\0",writer:"Jeff Kinney\0",price:7.00},
	{print:print_item_a, name:"The Dogist Puppies\0",writer:"Elias Weiss Friedman\0",price:15.67},
	{print:print_item_a, name:"The Rooster Bar\0",writer:"John Grisham\0",price:17.37},
	{print:print_item_a, name:"Ada Twist, Scientist\0",writer:"David Roberts\0",price:13.46},
};

void add_item_a()
{
	unsigned char choice;
	struct _item_a *item_t;

	while (1)
	{
		puts("--------------------------------");
		for(int i=0;i<(sizeof(item_a)/sizeof(struct _item_a));i++)
		{
			printf("[%c] ", 'A'+i);
			item_a[i].print(&item_a[i]);
		}
		puts("--------------------------------");
		printf("[n] back\n");

		printf("Which book? ");

		choice = get_char();

		if (choice == 'n') return ;

		if ((choice - 'A')>=0 && (choice - 'A')<(sizeof(item_a)/sizeof(struct _item_a)))
		{
			item_t = malloc(sizeof(struct _item_a));
			item_t->print = print_item_a;
			printf("remark: ");
			get_str(item_t->mark, 32, '\n');
			strncpy(item_t->name,item_a[choice - 'A'].name,strlen(item_a[choice - 'A'].name));
			strncpy(item_t->writer,item_a[choice - 'A'].writer,strlen(item_a[choice - 'A'].writer));
			item_t->price = item_a[choice - 'A'].price;
			item_t->print(item_t);//
			if (check_cart((void *)item_t)==0)
			{
				free(item_t);
			}
			else
			{
				cart_total += item_t->price;
			}
		}

	}
}

struct _item_b
{
  char mark[32];
  char name[32];
  char actor[32];
  char padding[96];
  double price;
  char *message;
  void (*print)();
  int pad;
};

void print_item_b(struct _item_b *item_t)
{
	printf("Name: %s\t\tActor: %s\t\tPrice: $%.2f\n", item_t->name, item_t->actor, item_t->price);
}

struct _item_b item_b[] = {
	{print:print_item_b, name:"Wonder Woman\0",actor:"Gal Gadot\0",price:14.99},
	{print:print_item_b, name:"Your Name\0",actor:"Michael Sinterniklaas\0",price:19.96},
	{print:print_item_b, name:"Spider-Man\0",actor:"Tom Holland\0",price:19.99},
	{print:print_item_b, name:"Cars 3\0",actor:"Owen Wilson\0",price:44.99},
	{print:print_item_b, name:"Despicable Me 3\0",actor:"None\0",price:9.47},
	{print:print_item_b, name:"Game of Thrones\0",actor:"Various\0",price:39.96},
};

void add_item_b()
{
	unsigned char choice;
	struct _item_b *item_t;

	while (1)
	{
		puts("--------------------------------");
		for(int i=0;i<(sizeof(item_b)/sizeof(struct _item_b));i++)
		{
			printf("[%c] ", 'A'+i);
			item_b[i].print(&item_b[i]);
		}
		puts("--------------------------------");
		printf("[n] back\n");

		printf("Which moive? ");

		choice = get_char();

		if (choice == 'n') return ;

		if ((choice - 'A')>=0 && (choice - 'A')<(sizeof(item_b)/sizeof(struct _item_b)))
		{
			item_t = malloc(sizeof(struct _item_b));
			item_t->print = print_item_b;
			printf("remark: ");
			get_str(item_t->mark, 32, '\n');
			strncpy(item_t->name,item_b[choice - 'A'].name,strlen(item_b[choice - 'A'].name));
			strncpy(item_t->actor,item_b[choice - 'A'].actor,strlen(item_b[choice - 'A'].actor));
			item_t->price = item_b[choice - 'A'].price;
			item_t->print(item_t);//
			if (check_cart((void *)item_t)==0)
			{
				free(item_t);
			}
			else
			{
				cart_total += item_t->price;
			}
		}

	}
}

struct _item_c
{
  char mark[32];
  char name[64];
  char desc[96];
  double price;
  float discount;
  void (*print)();
  int pad;
};

void print_item_c(struct _item_c *item_t)
{
	printf("Name: %s\t\tDescription: %s\t\tPrice: $%.2f\t\tDiscount: %.2f\n", item_t->name, item_t->desc, item_t->price, item_t->discount);
}

struct _item_c item_c[] = {
	{print:print_item_c, name:"HP Stream Laptop PC 14-ax010nr\0",desc:"(Intel Celeron N3060, 4 GB RAM, 32 GB eMMC) with Office 365 Personal for one year\0",price:219.00,discount:9.0},
	{print:print_item_c, name:"Huawei MateBook X Signature Edition Ultraslim Laptop\0",desc:"Intel Core i7-7500U, 8GB RAM, 512GB SSD, Fingerprint, Office 365 Personal, MateDock v2.0\0",price:1279.90,discount:8.8},
	{print:print_item_c, name:"MSI GT73VR TITAN PRO-1005 \0",desc:"17.3\" 120Hz 5ms Hardcore Gaming Laptop i7-7700HQ GTX 1080 8G 16GB 512GB SSD + 1TB, Black-Red\0",price:2499.99,discount:8.0},
	{print:print_item_c, name:"Lenovo Flex 5 2-in-1 Laptop\0",desc:"Intel 7th Gen i7-7500U, 16GB DDR4 RAM, 1TB HDD + 256GB PCIe SSD, NVIDIA GeForce 940MX\0",price:1149.99,discount:8.8},
	{print:print_item_c, name:"Acer Nitro 5, Intel Core i5-7300HQ\0",desc:"GeForce GTX 1050 Ti, 15.6\" Full HD, 8GB DDR4, 256GB SSD, AN515-51-55WL\0",price:699.99,discount:9.0},
	{print:print_item_c, name:"HP Pavilion Power 15-inch Laptop\0",desc:"Intel Core i7-7700HQ, AMD Radeon RX 550, 12GB RAM, 1TB hard drive, Windows 10\0",price:829.56,discount:9.8}
};

void add_item_c()
{
	unsigned char choice;
	struct _item_c *item_t;

	while (1)
	{
		puts("--------------------------------");
		for(int i=0;i<(sizeof(item_c)/sizeof(struct _item_c));i++)
		{
			printf("[%c] ", 'A'+i);
			item_c[i].print(&item_c[i]);
		}
		puts("--------------------------------");
		printf("[n] back\n");

		printf("Which book? ");

		choice = get_char();

		if (choice == 'n') return ;

		if ((choice - 'A')>=0 && (choice - 'A')<(sizeof(item_c)/sizeof(struct _item_c)))
		{
			item_t = malloc(sizeof(struct _item_c));
			item_t->print = print_item_c;
			printf("remark: ");
			get_str(item_t->mark, 32, '\n');
			strncpy(item_t->name,item_c[choice - 'A'].name,strlen(item_c[choice - 'A'].name));
			strncpy(item_t->desc,item_c[choice - 'A'].desc,strlen(item_c[choice - 'A'].desc));
			item_t->price = item_c[choice - 'A'].price;
			item_t->discount = item_c[choice - 'A'].discount;
			item_t->print(item_t);//

			if (check_cart((void *)item_t)==0)
			{
				free(item_t);
			}
			else
			{
				cart_total += item_t->price*(item_t->discount/10);
			}
		}

	}
}
struct _item_d
{
  char mark[32];
  char name[32];
  char desc[64];
  char padd[64];
  double price;
  char *message;
  void (*print)();
  char details[128];
};

void print_item_d(struct _item_d *item_t)
{
	printf("Name: %s\t\tDesc: %s\t\tPrice: $%.2f\t\tDetails: %s\n", item_t->name, item_t->desc, item_t->price, item_t->details);
}

struct _item_d item_d[] = {
	{print:print_item_d, name:"DYMO\0",desc:"DYMO LabelManager 280 Rechargeable Hand-Held Label Maker\0",price:14.87,details:"The DYMO LabelManager 280 prints durable, water-resistant labels that make it easy to access files, charts, tools, and more. \0"},
	{print:print_item_d, name:"Ontel\0",desc:"SpeedOut Damaged Screw Extractor & Bolt Extractor Set\0",price:6.88,details:"If you've ever stripped a screw or broken off the head of a bolt, you know how frustrating that can be. \0" },
	{print:print_item_d, name:"Top Greener\0",desc:"TOPGREENER TU2154A High Speed USB Charger Outlet\0",price:16.99,details:"USB Wall Charger, Electrical Outlet with USB, 15A TR Receptacle, Screwless Wall Plate, for iPhone X, iPhone 8/8 Plus \0" },
	{print:print_item_d, name:"BLENDX\0",desc:"Universal Sockets Metric Wrench Power Drill Adapter Set \0",price:6.88,details:"Automatic standard and metric application; Unscrew various shapes nuts,screws,hooks,lag screws and bolt heads ect. \0" },
	{print:print_item_d, name:"Mitutoyo\0",desc:"Mitutoyo 64PKA075 Tool Kit, 0-1\" Range Dial Indicator\0",price:6.88,details:"This item's packaging may indicate what is inside. To cover it, select Ship in Amazon box on the checkout page. \0" },
};

void add_item_d()
{
	unsigned char choice;
	struct _item_d *item_t;

	while (1)
	{
		for(int i=0;i<(sizeof(item_d)/sizeof(struct _item_d));i++)
		{
			printf("[%c] ", 'A'+i);
			item_d[i].print(&item_d[i]);
		}

		printf("[n] back\n");

		printf("Which tool? ");

		choice = get_char();

		if (choice == 'n') return ;

		if ((choice - 'A')>=0 && (choice - 'A')<(sizeof(item_d)/sizeof(struct _item_d)))
		{
			item_t = malloc(sizeof(struct _item_d));
			item_t->print = print_item_d;
			printf("remark: ");
			get_str(item_t->mark, 32, '\n');
			strncpy(item_t->name,item_d[choice - 'A'].name,strlen(item_d[choice - 'A'].name));
			strncpy(item_t->desc,item_d[choice - 'A'].desc,strlen(item_d[choice - 'A'].desc));
			strncpy(item_t->details,item_d[choice - 'A'].details,strlen(item_d[choice - 'A'].details));
			item_t->price = item_d[choice - 'A'].price;
			item_t->print(item_t);//
			puts("leave a message?");
			item_t->message = get_message(0);
			if (check_cart((void *)item_t)==0)
			{
				free(item_t->message);
				free(item_t);
			}
			else
			{
				cart_total += item_t->price;
			}
		}

	}
}

struct _item_e
{
  char mark[32];
  char name[32];
  char desc[128];
  double price;
  char *message;
  void (*print)();
  int pad;
};

void print_item_e(struct _item_e *item_t)
{
	printf("Name: %s\t\tDescription: %s\t\tPrice: $%.2f\n", item_t->name, item_t->desc, item_t->price);
}

struct _item_e item_e[] = {
	{print:print_item_e, name:"Faith Beauty Eyelash\0",desc:"Advanced Formula for Longer and Thicker Lashes\0",price:22.99},
	{print:print_item_e, name:"Silk Sleep or Eye Mask\0",desc:"100% mulberry silk and floss-soft, smooth, lightweight and comfortable\0",price:14.99},
	{print:print_item_e, name:"Faith Beauty Eyelash\0",desc:"The magnetic eyelashes sets are made of high quality material.\0",price:10.11},
	{print:print_item_e, name:"Premium Quality Blackhead Remover\0",desc:"The Mask can deeply clean stubborn blackheads on face without any harms.\0",price:14.99},
	{print:print_item_e, name:"Toenail Nipper for Thick\0",desc:"The toenail nipper was designed to easily clip nails of all sizes.\0",price:22.99},
	{print:print_item_e, name:"Magnetic Eyelashes\0",desc:"The toenail nipper was designed to easily clip nails of all sizes\0",price:18.95}

};

void add_item_e()
{
	unsigned char choice;
	struct _item_e *item_t;

	while (1)
	{
		for(int i=0;i<(sizeof(item_e)/sizeof(struct _item_e));i++)
		{
			printf("[%c] ", 'A'+i);
			item_e[i].print(&item_e[i]);
		}

		printf("[n] back\n");

		printf("Which beauty? ");

		choice = get_char();

		if (choice == 'n') return ;

		if ((choice - 'A')>=0 && (choice - 'A')<(sizeof(item_e)/sizeof(struct _item_e)))
		{
			item_t = malloc(sizeof(struct _item_e));
			item_t->print = print_item_e;
			printf("remark: ");
			get_str(item_t->mark, 32, '\n');
			strncpy(item_t->name,item_e[choice - 'A'].name,strlen(item_e[choice - 'A'].name));
			strncpy(item_t->desc,item_e[choice - 'A'].desc,strlen(item_e[choice - 'A'].desc));
			item_t->price = item_e[choice - 'A'].price;
			item_t->print(item_t);//
			puts("leave a message?");
			item_t->message = get_message(0xe0);
			if (check_cart((void *)item_t)==0)
			{
				free(item_t->message);
				free(item_t);
			}
			else
			{
				cart_total += item_t->price;
			}
		}

	}
}

struct _item_f
{
  char mark[32];
  char name[128];
  char desc[32];
  double price_low;
  double price_high;
  void (*print)();
  int star;
  char *message;
  int pad;
};

void print_item_f(struct _item_f *item_t)
{
	printf("Name: %s\t\tPrice: $%.2f~$%.2f\t\tStar: %d\n", item_t->name, item_t->price_low, item_t->price_high, item_t->star);
}

struct _item_f item_f[] = {
	{print:print_item_f, name:"Save on 2017 Chrome Soft and Chrome Soft X Golf Balls\0",price_low:31.99,price_high:72.64,star:5},
	{print:print_item_f, name:"Up to 40% Off Select NCAA Headwear Styles\0",price_low:4.19,price_high:24.00,star:5},
	{print:print_item_f, name:"Up to 40% Off Select NFL homeware and headwear\0",price_low:6.00,price_high:100.17,star:4},
	{print:print_item_f, name:"Up to 40% Off Select NFL Men's Apparel Styles\0",price_low:4.79,price_high:179.99,star:5},
	{print:print_item_f, name:"Save on GoSports Giant Toppling Tower with Bonus Rules and more\0",price_low:30.68,price_high:84.98,star:5},
	{print:print_item_f, name:"Save on Hodgman Waders, Boots and More\0",price_low:11.90,price_high:91.46,star:4},
	{print:print_item_f, name:"Save on Mojo Outdoors Teal Duck Decoy and more\0",price_low:41.31,price_high:123.14,star:4},
	{print:print_item_f, name:"Up to 25% off select OTS NFL knits\0",price_low:10.50,price_high:13.50,star:5}
};

int isValidEmail(char *pcEmailaddr)  
{  
    char user[128] = {0};  
    char site[128] = {0};  
  
    if(NULL == pcEmailaddr)  
    {  
        return 0;  
    }  
      
    sscanf(pcEmailaddr, "%[0-9a-zA-Z-_.]%[@0-9a-zA-Z._]", user,site);  
  
    if(0 < strlen(user) && 0 < strlen(site) && ((strlen(user) + strlen(site)) == strlen(pcEmailaddr)))  
    {  
        if('@' == site[0])  
        {  
            return 1;  
        }  
    }  
  
    return 0;  
}  

void add_item_f()
{
	unsigned char choice;
	struct _item_f *item_t;

	while (1)
	{
		puts("--------------------------------");
		for(int i=0;i<(sizeof(item_f)/sizeof(struct _item_f));i++)
		{
			printf("[%c] ", 'A'+i);
			item_f[i].print(&item_f[i]);
		}
		puts("--------------------------------");
		printf("[n] back\n");

		printf("Which sport? ");

		choice = get_char();

		if (choice == 'n') return ;

		if ((choice - 'A')>=0 && (choice - 'A')<(sizeof(item_f)/sizeof(struct _item_f)))
		{
			item_t = malloc(sizeof(struct _item_f));
			item_t->print = print_item_f;
			printf("remark: ");
			get_str(item_t->mark, 32, '\n');
			strncpy(item_t->name,item_f[choice - 'A'].name,strlen(item_f[choice - 'A'].name));
			item_t->price_low = item_f[choice - 'A'].price_low;
			item_t->price_high = item_f[choice - 'A'].price_high;
			item_t->star = item_f[choice - 'A'].star;
			item_t->print(item_t);//
			puts("leave your email?");
			item_t->message = get_message(0x40);
			if (isValidEmail(item_t->message)==0)
			{
				puts("Not valid!");
				free(item_t->message);
			}
			if (check_cart((void *)item_t)==0)
			{
				free(item_t->message);
				free(item_t);
			}
			else
			{
				cart_total += item_t->price_low;
			}
		}

	}
}

void items_menu()
{
	puts("--------------------------------");
	puts("[a] Books & Audible");
	puts("[b] Movies, Music & Games");
	puts("[c] Electronics, Computers & Office");
	puts("[d] Home, Garden & Tools");
	puts("[e] Beauty & Health");
	puts("[f] Sports & Outdoors");
	puts("--------------------------------");
	puts("[n] back");
	printf(": ");
}

void have_a_look()
{
	unsigned char choice;
	while (1) 
	{
		items_menu();
		choice = get_char();
		switch (choice) 
		{
			case 'a':
				add_item_a();
				break;
			case 'b':
				add_item_b();
				break;
			case 'c':
				add_item_c();
				break;
			case 'd':
				add_item_d();
				break;
			case 'e':
				add_item_e();
				break;
			case 'f':
				add_item_f();
				break;
			case 'n':
				return ;
			default:
				puts("invalid choice !");
				break;
		}

	}

}

void remove_an_item()
{
	int found = -1;
	char buf[32];
	printf("Item name to remove? ");

	get_str(buf, 64, '\n');

	for (int i = 0; i < 99; ++i)
	{
		if (cart[i] && strcmp(((long *)cart[i]),buf)==0)
		{
			printf("$%.2lf\n",(double)(*((double *)cart[i]+24)));
			cart_total -= (double)(*((double *)cart[i]+24));
			cart_count -= 1;
			free(cart[i]);
			found = i;
		}
	}
	
	if (found == -1)
	{
		printf("Not found");
	}
	else
	{
		cart[found] = 0;
		puts("remove successfully!");
	}
	

}

void show_my_cart()
{
	for (int i = 0; i < cart_count; ++i)
	{
		if (cart[i]!=0)
		{
			(*(void  (*)())(*((long *)cart[i]+26)))(cart[i]);
		}
	}

	if (cart_count > 0)
		printf("Total is $%.2lf\n", cart_total);

}

void checkSum(unsigned long long cardNum) {

    int currentDigit;
    int count = 0;
    int oddSum = 0;
    int evenSum = 0;

    while (cardNum > 0) {

        currentDigit = cardNum % 10;

        if (count % 2 == 1){
            int doubled = currentDigit * 2;
            oddSum += doubled / 10 + doubled % 10;
        }
        else {
            evenSum += currentDigit;
        }

        count++;
        cardNum = cardNum / 10;

    }

    int total = oddSum + evenSum;
    if (total % 10 != 0) {
        printf("INVALID\n");
        exit(0);
    }

}

/* This part then returns the type of bank this card is from. */
char* findBank(unsigned long long cardNum) {
    char* bank = "";
    do {
        if (cardNum == 34 || cardNum == 37) {
            bank = "AMEX";
            break;
        } else if (51 <= cardNum && cardNum <= 55) {
            bank = "MASTERCARD";
            break;
        } else if (cardNum == 4) {
            bank = "VISA";
            break;
        } else {
            bank = "INVALID";
        }
        cardNum = cardNum / 10;
    } while (cardNum > 0);

    if(strcmp(bank, "INVALID") == 0){
        printf("INVALID\n");
        exit(0);
    }

    return bank;
}

int validCardLength(unsigned long long cardNum, char* bank) {

    int size = 0;

    while (cardNum > 0) {
        size ++;
        cardNum = cardNum / 10;
    }
    if (strcmp(bank, "AMEX") == 0){
        if (size == 15)
            printf("%s\n", bank);
    } else if (strcmp(bank, "MASTERCARD") == 0) {
        if (size == 16)
            printf("%s\n", bank);
    } else if (strcmp(bank, "VISA") == 0) {
        if (size == 13 || size == 16)
            printf("%s\n", bank);
    } else {
        printf("INVALID\n");
        return 0;
    }

    return 1;
}

void have_the_bill()
{
	printf("Number: ");
    unsigned long long cardNum;
    scanf("%lld\n", &cardNum);
    checkSum(cardNum);
    char* bank = findBank(cardNum);
    if (validCardLength(cardNum, bank)==0)
		printf("Not valid card");
}

void menu() {
	puts("======= You can =======");
	puts("1. have a look ->");
	puts("2. remove an item ->");
	puts("3. show my shopping cart ->");
	puts("4. have the bill ->");
	puts("5. quit ->");
	puts("=======================");
	printf("your choice: ");
}

void chall_main() {
	unsigned choice;

	while (1) 
	{
		menu();
		choice = get_choice();
		switch (choice) 
		{
			case 1:
				have_a_look();
				break;
			case 2:
				remove_an_item();
				break;
			case 3:
				show_my_cart();
				break;
			case 4:
				have_the_bill();
				break;
			case 5:
				puts("see you later!");
				exit(0);
			default:
				puts("invalid choice !");
				break;
		}

	}
}

void sigalrm_fn(int sig)
{
        puts("\n[!] bye!");
        exit(-1);
}

void initial() 
{
	signal(SIGALRM, sigalrm_fn);
	alarm(0x10000);

	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);

	printf("Welcome to L-T shopping mall!\n");
}

int main(int argc, char* argv[])
{

	initial();
	chall_main();
	
	return 0;
}
