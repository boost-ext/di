using Ninject;

class Module : Ninject.Modules.NinjectModule {
    public override void Load() {
        Bind<int>().To(42);
    }
}

class ninject {
    static void Main(string[] args) {
        Ninject.IKernel kernel = new StandardKernel(new Module());
        kernel.Get<int>();
    }
} 


