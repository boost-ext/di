using Ninject;

interface I { void dummy(); }; class Impl : I { public Impl() { } void I.dummy() { } }

class Module : Ninject.Modules.NinjectModule {
    public override void Load() {
        //Bind<I>().To<Impl>();
    }
}

class ninject {
    static void Main(string[] args) {
        Ninject.IKernel kernel = new StandardKernel(new Module());
        I object = kernel.Get<I>();
        object->dummy();
    }
} 

