import torch
import torchvision
from PIL import Image
import os
from Model import LinerModel
from sklearn import metrics
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from torchsummary import summary

def tensor2image(tensor: torch.Tensor(), image_path: str):
    transform = torchvision.transforms.ToPILImage()
    image = transform(tensor)
    image.save(image_path)


def load_model(model_path: str, device: str) -> torch.nn.Module:
    if not os.path.exists(model_path):
        raise("模型路径不存在")
    checkpoint = torch.load(model_path)
    model = LinerModel(28, 28).to(device)
    model.load_state_dict(checkpoint['model'])
    return model


def get_pair(predict, true):
    y = torch.argmax(predict, dim=-1)
    y_list = y.cpu().numpy().tolist()
    true_list = true.cpu().numpy().tolist()
    return y_list, true_list


def test_all(model):
    model.eval()
    test_transform = torchvision.transforms.Compose([
        torchvision.transforms.ToTensor(),
        torchvision.transforms.Normalize([0.5], [0.5])
    ])
    DATA_ROOT = os.path.abspath(os.path.join('cource-neural-network',
                                             'minist'))  # MINIST 数据集存放位置
    minist_datasets_test = torchvision.datasets.MNIST(DATA_ROOT, train=False,
                                                      transform=test_transform,
                                                      target_transform=None,
                                                      download=True)
    test_dataloader = torch.utils.data.DataLoader(
        minist_datasets_test, batch_size=128, shuffle=False)
    predict_list = []
    true_List = []
    total_loss = 0.
    cri = torch.nn.CrossEntropyLoss()
    for i, batch in enumerate(test_dataloader):
        out = model(batch[0].to(device))
        loss = cri(out, batch[1].to(device))
        predit, true = get_pair(out, batch[1].to(device))
        predict_list.extend(predit)
        true_List.extend(true)
        total_loss += loss.item()
    acc = metrics.accuracy_score(predict_list, true_List)
    print("****test loss"+str(total_loss/i)+" acc: "+str(acc)+"****")
    return acc


def test(model):
    model.eval()
    test_transform = torchvision.transforms.Compose([
        torchvision.transforms.ToTensor(),
        torchvision.transforms.Normalize([0.5], [0.5])
    ])
    DATA_ROOT = os.path.abspath(os.path.join('cource-neural-network',
                                             'minist'))  # MINIST 数据集存放位置
    minist_datasets_test = torchvision.datasets.MNIST(DATA_ROOT, train=False,
                                                      transform=test_transform,
                                                      target_transform=None,
                                                      download=True)
    test_dataloader = torch.utils.data.DataLoader(
        minist_datasets_test, batch_size=128, shuffle=False)
    cri = torch.nn.CrossEntropyLoss()
    array = np.zeros([10, 10])
    for i, batch in enumerate(test_dataloader):
        out = model(batch[0].to(device))
        predit, true = get_pair(out, batch[1].to(device))

        for j in range(len(batch[0])):
            if predit[j] != true[j]:
                array[predit[j]][true[j]] += 1
                folder = os.path.join(DATA_ROOT, 'image')
                if not os.path.exists(folder):
                    os.makedirs(folder)
                path = os.path.join(folder, str(
                    i)+'-'+str(predit[j])+'-'+str(true[j])+'.jpg')
                tensor2image(batch[0][j], path)
    out = np.argmax(array, axis=-1)
    sns.heatmap(array, cmap='Reds',annot=True)
    plt.title('热力图')
    plt.show()
    print()


if __name__ == "__main__":
    model_path = os.path.join(os.path.abspath(
        '.'), 'cource-neural-network', 'minist', 'checkpoint', 'best.pt')
    device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
    model = load_model(model_path, device)
    summary(model,(1,28,28),device=device.type,col_names=["kernel_size", "output_size", "num_params", "mult_adds"])
    test(model)
