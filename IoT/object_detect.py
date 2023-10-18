import torch
import tkinter as tk
from tkinter import filedialog
from PIL import Image, ImageTk
from ultralytics import YOLO
import os

model_path = "<path_to_model>"
print("Starting model loading...")
model = YOLO(model_path)
if model:
    print("Model loaded successfully!")
else:
    print("Failed to load the model.")
device = 'cuda' if torch.cuda.is_available() else 'cpu'

from PIL import ImageFilter

def predict_image(image_path):
    if not os.path.exists(image_path):
        print(f"File not found: {image_path}")
        return None

    img = Image.open(image_path)
    results_list = model(img)  

    if len(results_list) > 0:
        results = results_list[0]
        results_img = results.orig_img  
        return Image.fromarray(results_img)

    return img

panel = None

def open_file():
    global panel
    file_path = filedialog.askopenfilename()
    img = predict_image(file_path)
    if img:
        img = img.resize((500, 500), Image.LANCZOS)
        img = ImageTk.PhotoImage(img)
        if panel is None:
            panel = tk.Label(window, image=img)
            panel.image = img
            panel.grid(row=0, column=0, padx=10, pady=10)
        else:
            panel.config(image=img)
            panel.image = img
    else:
        print("Error processing the image.")


window = tk.Tk()
window.title("YOLOv8 Object Detection")
open_button = tk.Button(window, text="Open Image", command=open_file)
open_button.grid(row=1, column=0, padx=10, pady=10)

window.mainloop()
