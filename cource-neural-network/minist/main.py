import os
import torch
import torchvision
import tqdm
from Model import LinerModel,CnnModel
from sklearn import metrics

def get_pair(predict, true):
    y = torch.argmax(predict, dim=-1)
    y_list = y.cpu().numpy().tolist()
    true_list = true.cpu().numpy().tolist()
    return y_list, true_list


def test(model, dataloader):
    model.eval()
    predict_list = []
    true_List = []
    total_loss = 0.
    cri = torch.nn.CrossEntropyLoss()
    for i, batch in enumerate(dataloader):
        out = model(batch[0].to(device))
        loss = cri(out, batch[1].to(device))
        predit, true = get_pair(out, batch[1].to(device))
        predict_list.extend(predit)
        true_List.extend(true)
        total_loss += loss.item()
    acc = metrics.accuracy_score(predict_list, true_List)
    print("****test loss"+str('%.3f' % (total_loss/i))+" acc: "+str(acc)+"****")
    return acc


DATA_ROOT = os.path.abspath(os.path.join('cource-neural-network',
                                         'minist'))  # MINIST 数据集存放位置
CKECKPOINT_ROOT = os.path.abspath(os.path.join('cource-neural-network',
                                         'minist','checkpoint'))
if not os.path.exists(CKECKPOINT_ROOT):
    os.makedirs(CKECKPOINT_ROOT)
train_transform = torchvision.transforms.Compose([
    #torchvision.transforms.RandomCrop((24,24)), #.CenterCrop((24, 24))
    torchvision.transforms.Resize((28, 28)),
    torchvision.transforms.ToTensor(),
    torchvision.transforms.Normalize([0.5], [0.5])
])
test_transform = torchvision.transforms.Compose([
    torchvision.transforms.ToTensor(),
    torchvision.transforms.Normalize([0.5], [0.5])
])
minist_datasets_train = torchvision.datasets.MNIST(DATA_ROOT, train=True,
                                                   transform=train_transform,
                                                   target_transform=None,
                                                   download=True)                                                
minist_datasets_test = torchvision.datasets.MNIST(DATA_ROOT, train=False,
                                                  transform=test_transform,
                                                  target_transform=None,
                                                  download=True)
train_dataloader = torch.utils.data.DataLoader(
    minist_datasets_train, batch_size=128, shuffle=True)
test_dataloader = torch.utils.data.DataLoader(
    minist_datasets_test, batch_size=128, shuffle=False)
device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
model = LinerModel(28, 28).to(device=device)
# model = CnnModel().to(device=device)
optim = torch.optim.SGD(model.parameters(), lr=1e-2,
                        momentum=0.9, weight_decay=1e-5)
sch = torch.optim.lr_scheduler.StepLR(optim, 10, gamma=0.1)
cri = torch.nn.CrossEntropyLoss()
best_acc = 0.
for j in range(30):
    model.train()
    predict_list = []
    true_List = []
    for i, batch in enumerate(train_dataloader):
        out = model(batch[0].to(device))
        loss = cri(out, batch[1].to(device))
        predit, true = get_pair(out, batch[1].to(device))
        predict_list.extend(predit)
        true_List.extend(true)
        optim.zero_grad()
        loss.backward()
        optim.step()
        if i % 50 == 0:
            acc = metrics.accuracy_score(predict_list, true_List)
            print("epoch"+str(j)+" loss="+str('%.3f' % loss.item())+" acc: "+str(acc))
            predict_list.clear()
            true_List.clear()
    test_acc = test(model, test_dataloader)
    if test_acc >best_acc:
        checkpoint = {
            "model" : model.state_dict(),
            "optim" : optim.state_dict(),
            "sch" : sch.state_dict(),
            "epoch" : j

        }
        torch.save(checkpoint,os.path.join(CKECKPOINT_ROOT,"best.pt"))
    sch.step()
    print("current lr = "+str(sch.get_lr()))
