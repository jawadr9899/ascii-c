import os

os.chdir("./images")
x =[im for im in os.listdir() if ".png" in im]
for i,k in enumerate(x):
    os.rename(k,f"{i}.png")