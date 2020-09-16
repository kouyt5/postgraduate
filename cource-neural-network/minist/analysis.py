import torch
import torchvision

def tensor2image(tensor:torch.Tensor(),image_path:str):
    transform = torchvision.transforms.ToPILImage()