#include <stdio.h>
#include <string.h>

#define max_length 50
#define max_items 20
#define max_sku 3000
#define max_ord 5000

typedef struct SKU{
    int skuId; //unique
    char name[max_length];
    char category[max_length];
    float price;
    int stock;
    int soldCount;
} SKU;

typedef struct Order{
    int orderId; //unique
    int orderTime; // YYYYMMDDHHMM
    int status; // 0=Placed 1=Delivered 2=Cancelled
    int deliveryTime; // 0 if NA
    int itemIds[max_items];
    int itemQty[max_items];
    int itemCount;
    float subtotal; // sum of price*qty
} Order;

SKU SKUList[max_sku];
Order OrderList[max_ord];
int skuCount = 0, orderCount = 0;

void Save_SKU_Data()
{
    FILE *f1 = fopen("sku_data.txt", "w");
    if (f1 == NULL)
    {
        printf("Error: Cannot open sku_data.txt for writing.\n");
        return;
    }

    fprintf(f1, "SKUCount: %d\n", skuCount);

    for (int i = 0; i < skuCount; i++)
    {
        fprintf(f1, "ID: %d\n", SKUList[i].skuId);
        fprintf(f1, "Name: %s\n", SKUList[i].name);
        fprintf(f1, "Category: %s\n", SKUList[i].category);
        fprintf(f1, "Price: %.2f\n", SKUList[i].price);
        fprintf(f1, "Stock: %d\n", SKUList[i].stock);
        fprintf(f1, "Sold: %d\n\n", SKUList[i].soldCount);
    }

    fclose(f1);
}

void Save_Order_Data()
{
    FILE *f2 = fopen("order_data.txt", "w");
    if (f2 == NULL)
    {
        printf("Error: Cannot open order_data.txt for writing.\n");
        return;
    }

    fprintf(f2, "OrderCount: %d\n", orderCount);

    for (int i = 0; i < orderCount; i++)
    {
        char statusStr[15];
        if (OrderList[i].status == 0)
            strcpy(statusStr, "Placed");
        else if (OrderList[i].status == 1)
            strcpy(statusStr, "Delivered");
        else
            strcpy(statusStr, "Cancelled");

        fprintf(f2, "OrderID: %d\n", OrderList[i].orderId);
        fprintf(f2, "Status: %s\n", statusStr);
        fprintf(f2, "OrderTime: %d\n", OrderList[i].orderTime);
        fprintf(f2, "DeliveryTime: %d\n", OrderList[i].deliveryTime);
        fprintf(f2, "Subtotal: %.2f\n", OrderList[i].subtotal);
        fprintf(f2, "Items:\n");

        for (int j = 0; j < OrderList[i].itemCount; j++)
        {
            fprintf(f2, "  %d x %d\n", OrderList[i].itemIds[j], OrderList[i].itemQty[j]);
        }

        fprintf(f2, "\n");
    }

    fclose(f2);
}

void Load_Data()
{
    FILE *f1 = fopen("sku_data.txt", "r");
    FILE *f2 = fopen("order_data.txt", "r");

    if (f1 != NULL)
    {
        fscanf(f1, "SKUCount: %d", &skuCount);

        for (int i = 0; i < skuCount; i++)
        {
            fscanf(f1, "\n\nID: %d", &SKUList[i].skuId);
            fscanf(f1, "\nName: %s", SKUList[i].name);
            fscanf(f1, "\nCategory: %s", SKUList[i].category);
            fscanf(f1, "\nPrice: %f", &SKUList[i].price);
            fscanf(f1, "\nStock: %d", &SKUList[i].stock);
            fscanf(f1, "\nSold: %d", &SKUList[i].soldCount);
        }

        fclose(f1);
    }
    else
    {
        skuCount = 0;
    }

    if (f2 != NULL)
    {
        fscanf(f2, "OrderCount: %d", &orderCount);

        for (int i = 0; i < orderCount; i++)
        {
            char statusStr[15];
            fscanf(f2, "\n\nOrderID: %d", &OrderList[i].orderId);
            fscanf(f2, "\nStatus: %s", statusStr);
            fscanf(f2, "\nOrderTime: %d", &OrderList[i].orderTime);
            fscanf(f2, "\nDeliveryTime: %d", &OrderList[i].deliveryTime);
            fscanf(f2, "\nSubtotal: %f", &OrderList[i].subtotal);
            fscanf(f2, "\nItems:");

            if (strcmp(statusStr, "Placed") == 0)
            {
                OrderList[i].status = 0;
            }
            else if (strcmp(statusStr, "Delivered") == 0)
            {
                OrderList[i].status = 1;
            }
            else
            {
                OrderList[i].status = 2;
            }

            OrderList[i].itemCount = 0;

            int flag=1;

            while (flag==1)
            {
                int id, qty;
                int c = fgetc(f2);

                if (c == '\n' || c == EOF)
                {
                    c = fgetc(f2);
                    if (c == '\n' || c == EOF)
                    {
                        flag=0;
                    }
                    ungetc(c, f2);
                }
                else
                {
                    ungetc(c, f2);
                }

                if (fscanf(f2, " %d x %d", &id, &qty) == 2)
                {
                    OrderList[i].itemIds[OrderList[i].itemCount] = id;
                    OrderList[i].itemQty[OrderList[i].itemCount] = qty;
                    OrderList[i].itemCount++;
                }
                else
                {
                    flag=0;
                }
            }
        }

        fclose(f2);
    }
    else
    {
        orderCount = 0;
    }

    printf("Data loaded successfully!\n");
}

void Save_All()
{
    Save_SKU_Data();
    Save_Order_Data();
    printf("All data saved successfully!\n");
}

void Add_SKU() 
{
    if (skuCount >= max_sku) 
    {
        printf("Catalog full! Cannot add more SKUs.\n");
        return;
    }

    SKU newSKU;
    
    printf("Enter SKU ID, Name, Category, Price, Stock: ");
    scanf("%d %s %s %f %d", &newSKU.skuId, newSKU.name, newSKU.category, &newSKU.price, &newSKU.stock);
    
    for (int i = 0; i < skuCount; i++)
    {
        if (SKUList[i].skuId == newSKU.skuId) 
        {
            printf("Duplicate SKU ID!\n");
            return;
        }
    }
    
    if (newSKU.price < 0 || newSKU.stock < 0) 
    {
        printf("Invalid data!\n");
        return;
    }

    newSKU.soldCount = 0;
    SKUList[skuCount++] = newSKU;
    printf("SKU added successfully.\n");
    Save_SKU_Data();
    /* printf("SKU ID: %d | Name: %s | Category: %s | Price: %.2f | Stock: %d\n",
        newSKU.skuId, 
        newSKU.name, 
        newSKU.category, 
        newSKU.price, 
        newSKU.stock);
    */
}

void Update_or_Delete_SKU(int skuId) 
{
    int flag=1;
    for (int i = 0; i < skuCount; i++) 
    {
        if (SKUList[i].skuId == skuId) 
        {
            flag=0;
            int choice;
            printf("1.Update  2.Delete : ");
            scanf("%d", &choice);

            if (choice == 1) 
            {
                int subChoice;
                printf("\nWhat do you want to update?\n");
                printf("1. Name\n2. Category\n3. Price\n4. Stock\n5. Multiple fields\nEnter your choice: ");
                scanf("%d", &subChoice);

                if (subChoice == 1) 
                {
                    printf("Enter new name: ");
                    scanf("%s", SKUList[i].name);
                } 

                else if (subChoice == 2) 
                {
                    printf("Enter new category: ");
                    scanf("%s", SKUList[i].category);
                }

                else if (subChoice == 3) 
                {
                    printf("Enter new price: ");
                    scanf("%f", &SKUList[i].price);
                    if (SKUList[i].price < 0) 
                    {
                        printf("Invalid price.\n");
                        return;
                    }
                }

                else if (subChoice == 4) 
                {
                    printf("Enter new stock: ");
                    scanf("%d", &SKUList[i].stock);
                    if (SKUList[i].stock < 0) 
                    {
                        printf("Invalid stock.\n");
                        return;
                    }
                }

                else if (subChoice == 5) 
                {
                    int fieldChoice = -1;
                    while (fieldChoice != 0)
                    {
                        printf("\nWhich field do you want to update?\n");
                        printf("1. Name\n2. Category\n3. Price\n4. Stock\n0. Done\nEnter choice: ");
                        scanf("%d", &fieldChoice);

                        if (fieldChoice == 1) 
                        {
                            printf("Enter new name: ");
                            scanf("%s", SKUList[i].name);
                        } 
                        else if (fieldChoice == 2) 
                        {
                            printf("Enter new category: ");
                            scanf("%s", SKUList[i].category);
                        } 
                        else if (fieldChoice == 3) 
                        {
                            printf("Enter new price: ");
                            scanf("%f", &SKUList[i].price);
                            if (SKUList[i].price < 0) 
                            {
                                printf("Invalid price.\n");
                            }
                        } 
                        else if (fieldChoice == 4) 
                        {
                            printf("Enter new stock: ");
                            scanf("%d", &SKUList[i].stock);
                            if (SKUList[i].stock < 0) 
                            {
                                printf("Invalid stock.\n");
                            }
                        }
                        else if (fieldChoice != 0)
                        {
                            printf("Invalid choice. Try again.\n");
                        }
                    }
                }

                else 
                {
                    printf("Invalid option.\n");
                    return;
                }
                printf("SKU updated successfully.\n");
                Save_SKU_Data();
            } 

            else if (choice == 2) 
            {
                for (int j = 0; j < orderCount; j++) 
                {
                    if (OrderList[j].status == 1) 
                    {
                        for (int k = 0; k < OrderList[j].itemCount; k++)
                        {
                            if (OrderList[j].itemIds[k] == skuId) 
                            {
                                printf("Cannot delete: delivered order exists.\n");
                                return;
                            }
                        }
                    }
                }

                char confirm;
                printf("Are you sure you want to delete SKU ID %d? (y/n): ", skuId);
                scanf(" %c", &confirm);

                if (confirm == 'n' || confirm == 'N') 
                {
                    printf("Deletion cancelled.\n");
                    return;
                }
                
                for (int j = i; j < skuCount - 1; j++)
                {
                    SKUList[j] = SKUList[j + 1];
                }
                skuCount--;
                printf("SKU deleted.\n");
                Save_SKU_Data();
            }
            
            else
            {
                printf("Invalid choice.\n");
                return;
            }
        }
    }

    if(flag==1)
    {
        printf("SKU not found.\n");
    }

    return;
}

// here place means moving to cart according to problem statement
void Place_Order(int itemIds[], int itemQty[], int itemCount, int orderTime) 
{
    if (orderCount >= max_ord) 
    {
        printf("Error: Order list full!\n");
        return;
    }

    Order newOrder;
    newOrder.orderId = orderCount + 1;
    newOrder.orderTime = orderTime;  
    newOrder.itemCount = itemCount;
    newOrder.subtotal = 0;

    if (newOrder.itemCount <= 0 || newOrder.itemCount > max_items) 
    {
        printf("Invalid item count!\n");
        return;
    }

    for (int i = 0; i < itemCount; i++)
    {
        int found = 0;
        for (int j = 0; j < skuCount; j++)
        {
            if (SKUList[j].skuId == itemIds[i] && itemQty[i] >0) 
            {
                found = 1;
                newOrder.itemIds[i] = itemIds[i];
                newOrder.itemQty[i] = itemQty[i];
                newOrder.subtotal = newOrder.subtotal + SKUList[j].price * itemQty[i];
            }
        }

        if (found == 0) 
        {
            printf("Invalid input %d\n", itemIds[i]);
            return;
        }
    }

    newOrder.status = 0;  // 0 = Placed
    OrderList[orderCount] = newOrder;
    orderCount++;
    printf("Order placed. ID: %d, Total: %.2f\n", newOrder.orderId, newOrder.subtotal);
    Save_Order_Data(); 
}

// deliver means buying confirmed according to problem statement
void Deliver_Order(int orderId, int deliveryTime)
{
    int orderIndex=-1;
    int SKUIndex=-1;

    for(int i=0; i<orderCount;i++)
    {
        if(OrderList[i].orderId== orderId)
        {
            orderIndex= i;
            i=orderCount;
        }
    }

    if (orderIndex == -1)
    {
        printf("Order ID %d not found!\n", orderId);
        return;
    }

    for(int i=0;i<OrderList[orderIndex].itemCount;i++)
    {
        for(int j=0; j<skuCount;j++)
        {
            if(OrderList[orderIndex].itemIds[i]== SKUList[j].skuId)
                {
                    SKUIndex=j;
                    j=skuCount;
                }
        }
        if(OrderList[orderIndex].itemQty[i]>SKUList[SKUIndex].stock)
        {
            printf("ERROR: Stock not sufficient for SKU ID %d\n", SKUList[SKUIndex].skuId);
            return;
        }
    }

    for(int i=0;i<OrderList[orderIndex].itemCount;i++)
    {
        for(int j=0; j<skuCount;j++)
        {
            if(OrderList[orderIndex].itemIds[i]== SKUList[j].skuId)
                {
                    SKUIndex=j;
                }
        }
        SKUList[SKUIndex].stock-=OrderList[orderIndex].itemQty[i];
        SKUList[SKUIndex].soldCount+=OrderList[orderIndex].itemQty[i];
    }

    OrderList[orderIndex].deliveryTime=deliveryTime;
    OrderList[orderIndex].status=1; // 1 = Deilvered    
    printf("Order %d delivered successfully! \n", orderId);
    Save_SKU_Data();   
    Save_Order_Data(); 
    return;
}

void Cancel_Order(int orderId) 
{
    int found = 0;

    for (int i = 0; i < orderCount; i++) 
    {
        if (OrderList[i].orderId == orderId) 
        {
            found = 1;

            if (OrderList[i].status == 1) 
            {
                printf("Cannot cancel. Order already delivered.\n");
                return;
            }
            else if (OrderList[i].status == 2) 
            {
                printf("Order already cancelled earlier.\n");
                return;
            }
            else if (OrderList[i].status == 0) 
            {
                char confirm;
                printf("Are you sure you want to cancel order %d? (y/n): ", orderId);
                scanf(" %c", &confirm);

                if (confirm == 'n' || confirm == 'N') 
                {
                    printf("Cancellation confirmed.\n");
                    return;
                }
                // mark as cancelled
                OrderList[i].status = 2;
                printf("Order cancelled successfully.\n");
                Save_Order_Data();

                // Delete the cancelled order from the list
                /*
                for (int j = i; j < orderCount - 1; j++) 
                {
                    OrderList[j] = OrderList[j + 1];
                }
                orderCount--;
                printf("Order cancelled and removed from records.\n");
                */
                return;
            }
        }
    }

    if (found == 0)
    {
        printf("Order not found.\n");
    }
}

void Search_SKU_ByName_Substring(char query[])
{
    for (int i = 0; query[i]!= '\0'; i++)
    {
        if (query[i] >= 'A' && query[i] <= 'Z')
        {
            query[i] = query[i] + 32;
        }
    }

    int found = 0;

    for (int i = 0; i < skuCount; i++)
    {
        char lowerName[max_length];
        int j = 0;

        while (SKUList[i].name[j] != '\0')
        {
            if (SKUList[i].name[j] >= 'A' && SKUList[i].name[j] <= 'Z')
                lowerName[j] = SKUList[i].name[j] + 32;
            else
                lowerName[j] = SKUList[i].name[j];
            j++;
        }
        lowerName[j] = '\0';

        if (strstr(lowerName, query)!= NULL)
        {
            printf("Found SKU: ID=%d | Name=%s | Category=%s | Price=%.2f | Stock=%d\n",
                   SKUList[i].skuId,
                   SKUList[i].name,
                   SKUList[i].category,
                   SKUList[i].price,
                   SKUList[i].stock);
            found = 1;
        }
    }

    if (found == 0)
    {
        printf("No SKU found matching %s.\n", query);
    }
}

void Sort_Orders_ByTime()
{
    if (orderCount <= 1)
    {
        printf("No sorting required — less than 2 orders.\n");
        return;
    }

    Order key;
    int j;

    for (int i = 1; i < orderCount; i++)
    {
        key = OrderList[i];
        j = i - 1;

        while (j >= 0 && (OrderList[j].orderTime > key.orderTime || (OrderList[j].orderTime == key.orderTime && OrderList[j].orderId > key.orderId)))
        {
            OrderList[j + 1] = OrderList[j];
            j--;
        }
        OrderList[j + 1] = key;
    }

    printf("Orders sorted successfully!\n");
}

void TopK_BestSellers(int K)
{
    if (skuCount == 0)
    {
        printf("No SKUs available.\n");
        return;
    }

    if (K <= 0)
    {
        printf("Invalid value of K.\n");
        return;
    }

    if (K > skuCount)
    {
        K = skuCount;
    }

    SKU tempList[max_sku];

    for (int i = 0; i < skuCount; i++)
    {
        tempList[i] = SKUList[i];
    }

    SKU temp;

    for (int i = 0; i < skuCount - 1; i++)
    {
        for (int j = i + 1; j < skuCount; j++)
        {
            if (tempList[i].soldCount < tempList[j].soldCount || (tempList[i].soldCount == tempList[j].soldCount && strcmp(tempList[i].name, tempList[j].name) > 0))
            {
                temp = tempList[i];
                tempList[i] = tempList[j];
                tempList[j] = temp;
            }
        }
    }

    printf("\nTop %d Bestselling SKUs:\n", K);
    printf("%-5s %-8s %-20s %-15s %-10s %-12s %-10s\n","Rank", "SKU_ID", "Name", "Category", "Price", "Sold Count", "Stock");

    for (int i = 0; i < K; i++)
    {
        printf("%-5d %-8d %-20s %-15s %-10.2f %-12d %-10d\n",
           i + 1,
           tempList[i].skuId,
           tempList[i].name,
           tempList[i].category,
           tempList[i].price,
           tempList[i].soldCount,
           tempList[i].stock);
    }

}

void ABC_Analysis_SoldCount()
{
    if (skuCount==0)
    {
        printf("No SKUs in catalog\n");
        return;
    }

    SKU tempSKUList[max_sku];
    for (int i=0; i<skuCount; i++)
    {
        tempSKUList[i] = SKUList[i];
    }

    SKU temp;
    for (int i=0; i<skuCount-1; i++)
    {
        for (int j=i+1; j<skuCount; j++)
        {
            if (tempSKUList[i].soldCount < tempSKUList[j].soldCount)
            {
                temp = tempSKUList[i];
                tempSKUList[i] = tempSKUList[j];
                tempSKUList[j] = temp;
            }
        }
    }

    int totalSold = 0;
    for (int i=0; i<skuCount; i++)
        totalSold += tempSKUList[i].soldCount;

    if (totalSold==0)
    {
        printf("No sold items to analyze.\n");
        return;
    }

    float cum = 0;
    int totalA = 0, totalB = 0, totalC = 0;
    int countA = 0, countB = 0, countC = 0;

    printf("ABC Analysis of SKUs (Based on Sold Count)\n");

    for (int i = 0; i < skuCount; i++)
    {
        cum += tempSKUList[i].soldCount;
        float percent = (cum / totalSold) * 100;
        char cat;

        if (percent <= 80)
        {
            cat = 'A';
            totalA += tempSKUList[i].soldCount;
            countA++;
        }
        else if (percent <= 95)
        {
            cat = 'B';
            totalB += tempSKUList[i].soldCount;
            countB++;
        }
        else
        {
            cat = 'C';
            totalC += tempSKUList[i].soldCount;
            countC++;
        }
    }

    printf("Category Totals:\n");
    printf("A (~80%%) - Units: %-6d  SKUs: %-4d\n", totalA, countA);
    printf("B (~15%%) - Units: %-6d  SKUs: %-4d\n", totalB, countB);
    printf("C (~5%%) - Units: %-6d  SKUs: %-4d\n", totalC, countC);
    printf("Total Units Sold = %d\n", totalSold);
}

int main()
{
    int choice = -1;
    Load_Data();

    printf("\nBlinkit Lite-Inventory & Order Management System\n");

    while (choice != 0)
    {
        printf("\nMENU\n");
        printf("1. Add SKU\n");
        printf("2. Update/Delete SKU\n");
        printf("3. Search SKU by name substring\n");
        printf("4. Place Order\n");
        printf("5. Deliver Order\n");
        printf("6. Cancel Order\n");
        printf("7. Sort Orders by Time\n");
        printf("8. Top K Bestsellers\n");
        printf("9. ABC Analysis (SoldCount)\n");
        printf("0. Exit\n");
        printf("\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        /*
        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n')
            continue;
        }
        */

        switch (choice)
        {
            case 1:
                Add_SKU();
                break;

            case 2:
            {
                int skuId;
                printf("Enter SKU ID to update/delete: ");
                scanf("%d", &skuId);
                Update_or_Delete_SKU(skuId);
                break;
            }

            case 3:
            {
                char query[50];
                printf("Enter substring to search: ");
                scanf("%s", query);
                Search_SKU_ByName_Substring(query);
                break;
            }

            case 4:
            {
                int itemCount, orderTime;
                printf("Enter number of items in order: ");
                scanf("%d", &itemCount);

                int itemIds[max_items], itemQty[max_items];

                for (int i = 0; i < itemCount; i++)
                {
                    printf("Enter SKU ID and Quantity for item %d: ", i + 1);
                    scanf("%d %d", &itemIds[i], &itemQty[i]);
                }

                printf("Enter order time (as integer): ");
                scanf("%d", &orderTime);

                Place_Order(itemIds, itemQty, itemCount, orderTime);
                break;
            }

            case 5:
            {
                int orderId, deliveryTime;
                printf("Enter Order ID to deliver: ");
                scanf("%d", &orderId);
                printf("Enter delivery time (as integer): ");
                scanf("%d", &deliveryTime);
                Deliver_Order(orderId, deliveryTime);
                break;
            }

            case 6:
            {
                int orderId;
                printf("Enter Order ID to cancel: ");
                scanf("%d", &orderId);
                Cancel_Order(orderId);
                break;
            }

            case 7:
                Sort_Orders_ByTime();
                break;

            case 8:
            {
                int k;
                printf("Enter value of K for Top-K Bestsellers: ");
                scanf("%d", &k);
                TopK_BestSellers(k);
                break;
            }

            case 9:
                ABC_Analysis_SoldCount();
                break;

            case 0:
                printf("Saving data and exiting...\n");
                Save_All();
                break;

            default:
                printf("Invalid choice. Try again.\n");
                break;
        }
    }

    return 0;
}