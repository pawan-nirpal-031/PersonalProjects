# Ticker : GOLD
# Metal price API key : fd22d87fa01fa1cd9e938dbdd35f2bac

import requests as req 
from datetime import date 
from bs4 import BeautifulSoup

def getCommodityPrice(Commodity="gold"):
    data = req.get("https://api.metalpriceapi.com/v1/latest?api_key=fd22d87fa01fa1cd9e938dbdd35f2bac&base=INR&currencies=EUR,XAU,XAG")
    #if(data):
    print(data.json())
        # soup = BeautifulSoup(data.text,'html.parser')
        # price = soup.find("div",class_="gold_silver_table right-align-content").find("tr",class_="odd_row").findAll("td")
        # return price[1].text
    #else:
        #return f"unable to fetch data for {Commodity}"

