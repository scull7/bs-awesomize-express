open Jest;

type t = {status: int};

external castCompleteToJson : Express.complete => Js.Json.t = "%identity";

external createJson : 'x => Js.Json.t = "%identity";

describe("Awesomize Express", () =>
  describe("make", () => {
    let schema = [|
      (
        "clientId",
        {
          read: Awesomize.Read.key("clientId"),
          sanitize: None,
          validate: [Awesomize.Validator.required],
          normalize: None,
        }: Awesomize.definition,
      ),
    |];
    let decoder = (_) => ();
    let encoder = (_) => createJson @@ [%raw "{clientId: 1}"];
    let handler = (_, _) => Js.Promise.resolve();
    let res: Express.Response.t = [%raw
      "{ status: (code) => ({ json: () => ({status: code}) }) }"
    ];
    let convertJsonToT = complete => {
      status:
        Json.Decode.field(
          "status",
          Json.Decode.int,
          castCompleteToJson(complete),
        ),
    };
    testPromise("success", () => {
      let req: Express.Request.t = [%raw "{clientId: 1}"];
      AwesomizeExpress.make(
        ~schema,
        ~decoder,
        ~encoder,
        ~handler,
        (),
        req,
        res,
      )
      |> Js.Promise.then_(result =>
           (
             switch (convertJsonToT @@ result) {
             | {status: 200} => pass
             | _ =>
               fail("not an expected result, expecting a status code of 200")
             }
           )
           |> Js.Promise.resolve
         );
    });
    testPromise("fail", () => {
      let req: Express.Request.t = [%raw "{djfalsfjdskalfjs: 1}"];
      AwesomizeExpress.make(
        ~schema,
        ~decoder,
        ~encoder,
        ~handler,
        (),
        req,
        res,
      )
      |> Js.Promise.then_(result =>
           (
             switch (convertJsonToT @@ result) {
             | {status: 400} => pass
             | _ =>
               fail("not an expected result, expecting a status code of 400")
             }
           )
           |> Js.Promise.resolve
         );
    });
  })
);
