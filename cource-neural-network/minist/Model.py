import torch
import torch.nn as nn

class LinerModel(nn.Module):
    def __init__(self,iamge_w:int,image_h:int):
        """
        模型输入为(N,1,28,28),线性模型期望输入为(N,L)
        因此首先需要将输入转化，这里模型仅为线性的组合
        """
        super().__init__()
        self.drop_rate = 0.2
        self.liner = nn.Sequential(
            nn.Linear(28*28,1024),
            nn.Dropout(p=self.drop_rate),
            nn.PReLU(),

            nn.Linear(1024,2048),
            nn.Dropout(p=self.drop_rate),
            nn.ReLU(),

            nn.Linear(2048,1024),
            nn.Dropout(p=self.drop_rate),
            nn.ReLU(),

            nn.Linear(1024,512),
            nn.Dropout(p=self.drop_rate),
            nn.ReLU(),

            nn.Linear(512,10),
            nn.Dropout(p=self.drop_rate),
            nn.ReLU(),
        )

    def forward(self, x): #(N,1,28,28)
        trans_x = torch.squeeze(x,1).view([x.size(0),x.size(2)*x.size(3)])
        out = self.liner(trans_x) #(N,10)
        # out = nn.Softmax(dim=-1)(out)
        return out


class CnnModel(nn.Module):
    def __init__(self):
        super().__init__()
        self.drop_rate = 0.2
        self.cnn = nn.Sequential(
            nn.Conv2d(1,64,3,stride=1,padding=1),
            nn.BatchNorm2d(64),
            nn.PReLU(),
            nn.Dropout(p=self.drop_rate),

            nn.Conv2d(64,256,3,stride=2,padding=1),
            nn.BatchNorm2d(256),
            nn.ReLU(),
            nn.Dropout(p=self.drop_rate),

            nn.Conv2d(256,512,3,stride=2,padding=1),
            nn.BatchNorm2d(512),
            nn.ReLU(),
            nn.Dropout(p=self.drop_rate),

            nn.Conv2d(512,1024,3,stride=2,padding=1),
            nn.BatchNorm2d(1024),
            nn.ReLU(),
            nn.Dropout(p=self.drop_rate),

            nn.Conv2d(1024,256,3,stride=2,padding=1),
            nn.BatchNorm2d(256),
            nn.ReLU(),
            nn.Dropout(p=self.drop_rate),
        )

        self.linear = nn.Linear(256*2*2,10)

    def forward(self,x):
        out = self.cnn(x)
        out = out.view(out.size(0),out.size(1)*out.size(2)*out.size(3))
        out = self.linear(out)
        return out

if __name__ == "__main__":
    x = torch.randn([8,1,28,28])
    # model = LinerModel(28,28)
    model = CnnModel()
    device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
    model.to(device)
    out = model(x.to(device))
    print()
