import dagger.*;

@Module
class AppModule {
    @Provides int provideint() { return 42; }
}

@Component(modules = AppModule.class)
interface AppComponent {
    int build();
}

public class dagger2 {
    public static void main(String[] args) {
        System.exit(Dagger_AppComponent.create().build() != 42 ? 1 : 0);
   }
}

