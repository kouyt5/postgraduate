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

if __name__ == "__main__":
    x = torch.randn([8,1,28,28])
    model = LinerModel(28,28)
    device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
    model.to(device)
    out = model(x.to(device))
    print()
